/* ---------------------------------------------------------------------*/
/* Librerias*/
#include "os.h"

#include "control.h"
#include "sensors.h"
#include "actuators.h"

#include <string.h>
/*Para SEND*/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> /*Para read*/
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

/* ---------------------------------------------------------------------*/
/* Cosntantes*/
#define k 1
#define EXPLORATION_ALTITUD 3
#define TOLERANCE 0.1

/* ---------------------------------------------------------------------*/
/* Variables generales*/

char buffer[128];

/*Variables para calcular la gravedad*/

float measured_gravity=1;

/* Variables para almacenar los valores de los sensores*/

float valuespeed;
float valueAlt1=10000, valueAlt2=10001, valueAlt3=10002;
int first_time_left=1;
int first_time_right=1;


//Optimal zone: Se almacena la posicion para aterrizar referenciado al sensor de derechas
float optimal_zone = -1; //Lo inizializamos a -1 para indicar que todavia no hemos encotrado la optimal zone

//Posiciones verticales y horitzontales
float vertical_position=EXPLORATION_ALTITUD;  //Inizializamos la posicion vertical que debe descender a EXPLORATION_ALTITUD
float horitzontal_position;

/* ---------------------------------------------------------------------*/
/* Inicializacion*/

int init_tasks()
{
    /*Tascas de sensores----------------------------------------------------------*/
    PTASK tspeed = NULL;
    PTASK tgravity = NULL;
 
    Task_create(&tspeed, "tspeed ", read_speed, NULL, 3, 3);
    Readyqueue_enqueue(tasks_queue, tspeed );

    Task_create(&tgravity, "tspeed ", calculate_gravity, NULL, 0, 0);
    Readyqueue_enqueue(tasks_queue, tgravity );

    //PTASK tsensores_altura = NULL;
    //Task_create(&tsensores_altura, "read_senosres", read_sensores_altura , NULL, 500, 500);
    //Readyqueue_enqueue(tasks_queue, tsensores_altura);
    


    /*Tascas de control---------------------------------------------------------*/
    PTASK tlanding = NULL;
    Task_create(&tlanding, "Control", landing_control, NULL, 3, 3);
    Readyqueue_enqueue(tasks_queue, tlanding);
    return (0);
}

/* ---------------------------------------------------------------------*/
/* Instrucciones para control*/

//Control general aterrizaje
void landing_control() 
{
    
    go2vertical_positon();

    //Mientras no tengamos la optimal_zone, debemos seguir buscando
    if(optimal_zone==-1)
    {
        read_sensores_altura();
        void_optimal_zone(); //Verificamos si tenemos un optimal zone
        vertical_position=EXPLORATION_ALTITUD;  

        if (first_time_left)
        {
            write_actuator(drift_fd, "left");
            usleep(500000);
            write_actuator(drift_fd, "release");
            first_time_left=0;
        }
    }

    //Si ya tenemos la optimal_zone, nos dirigimos a ella
    else
    {   
        if (first_time_right)
        {
            write_actuator(drift_fd, "right");
            usleep(500000);
            write_actuator(drift_fd, "release");
            first_time_right=0;
        }
        vertical_position=0; //Aterriza
    }   
}

//Verificamos si es una posicion correcta para aterrizar
//Lo referenciado respecto sensor a la derecha
//Lo verificamos con tolerancias ya que los sensores no miden todos al mismo instante
void void_optimal_zone(){  
    
    if(((valueAlt1-valueAlt2)<TOLERANCE )&& ((valueAlt1-valueAlt2)> -TOLERANCE ))//Vigilar que la primera vez....
    {
        if(((valueAlt1-valueAlt3)<TOLERANCE  )&& ((valueAlt1-valueAlt3)> -TOLERANCE ))
        {
            if(((valueAlt2-valueAlt3)<TOLERANCE  )&& ((valueAlt2-valueAlt3)> -TOLERANCE ))
            {
                optimal_zone = 1;               
            }
        }
    }
}


void go2vertical_positon() //Pasamos como parametro la altitud donde debe bajar
{    
    read_socket_passive_float(alt3_fd, &valueAlt3);
    float v=0.0,e=0.0;
    
    v=valueAlt3-vertical_position; //velocidad(altitud)=k*valuealt1 (En este caso k=1)
    e=valuespeed -v;        

    if (e>4)
    {
        write_actuator(pump1_fd, "start");
        write_actuator(pump2_fd, "start");

    }

    else if (4>e && e>0.5)
    {
        if (measured_gravity<5 && valuespeed<2)
        {
            sprintf(buffer, "speed: %f\n",valuespeed);
             write(2, buffer, strlen(buffer));
            write_actuator(pump1_fd, "turnoff");
            write_actuator(pump2_fd, "turnoff");
        }
        else
        {
            write_actuator(pump1_fd, "start");
            write_actuator(pump2_fd, "turnoff");
        }
                
    }
    else
    {
        write_actuator(pump1_fd, "turnoff");
        write_actuator(pump2_fd, "turnoff");
    }
}

/*Instrucciones para los actuadores*/

int write_actuator(int fd, char *message)
{
    int length = strlen(message) + 1;
    send(fd, (char *)&length, sizeof(int), 0);
    send(fd, message, length, 0);
}

/* ---------------------------------------------------------------------*/
/*Lectura de sensores*/
void calculate_gravity()
{
    struct timeval t_actual, t_previous;
    float speed_previous, speed_actual;
    gettimeofday(&t_actual, NULL);
    read_speed();
    speed_previous=valuespeed;
    
    sleep(1);
    gettimeofday(&t_previous, NULL);
    read_speed();
    speed_actual=valuespeed;
    
    measured_gravity=-(speed_actual-speed_previous)*1000000/((1000000*t_actual.tv_sec+t_actual.tv_usec)-(1000000*t_previous.tv_sec+t_previous.tv_usec));
    sprintf(buffer, "Gravity: %f\n",measured_gravity);
    write(2, buffer, strlen(buffer));
     
}
void read_speed()
{  
    read_pipe_active_float(speed_fd_read, &valuespeed);
}

void read_sensores_altura()
{  
    read_socket_passive_float(alt1_fd, &valueAlt1);
    read_socket_passive_float(alt2_fd, &valueAlt2);
    read_socket_passive_float(alt3_fd, &valueAlt3);
}

int read_pipe_active_float(int fd, float *value)
{
    read(fd, value, sizeof(float));
    return 0;
}

int read_socket_passive_float(int fd, float *value)
{
    char c = 0;
    send(fd, (char *)&c, sizeof(char), 0);
    recv(fd, value, sizeof(float), 0);
    return 0;
}

/* ---------------------------------------------------------------------*/
/*
    sprintf(buffer, "Alt1: %f\n",valueAlt1-valueAlt2);
    write(2, buffer, strlen(buffer));
    sprintf(buffer, "Alt2: %f\n",valueAlt1-valueAlt3);
    write(2, buffer, strlen(buffer));
    sprintf(buffer, "Alt3: %f\n",valueAlt3-valueAlt2);
    write(2, buffer, strlen(buffer));
    sprintf(buffer, "optimal zone::  %f\n", optimal_zone);
    write(2, buffer, strlen(buffer));
*/