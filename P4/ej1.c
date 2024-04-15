/*Sesion 4: Memoria compartida*/
/*Ej1: Vector global de long de 10 posiciones
    - creamos un thread
    - el proceso padre y el thread recorreran el vector 1000 veces incrementando +1 cada posicion */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);

/*Estructura mutex*/
pthread_mutex_t mutex;

long int array[10]={0};

void* func(void* arg)
{
    int i,j;
    for (i=0; i<10000; i++){
        for (j=0; j<10; j++)
        {
            pthread_mutex_lock(&mutex);
            array[j]++;
            pthread_mutex_unlock(&mutex);
        }    
    }
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char buffer[128];
    
    pthread_t id;
    void * endcode;

    /* Inicializacion del mutex*/
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&id, NULL, func, NULL);

    int k,m;
    for (k=0; k<10000; k++){
        for (m=0; m<10; m++)
        {
            pthread_mutex_lock(&mutex);
            array[m]++;
            pthread_mutex_unlock(&mutex);
        } 
    }

    pthread_join(id,(void **)&endcode);

    for (m=0; m<10; m++)
    {
        sprintf(buffer, "[%ld]", array[m]);
        write(1, buffer, strlen(buffer));    
    } 
  write(1, "\nAcabado\n", 9); 
  return (EXIT_SUCCESS);
}
/* Resultado: 20000 en todos */