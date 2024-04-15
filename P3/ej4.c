/*Sesion 3: threads*/
/*Ej4: Vector global de long de length posiciones
    - creamos el mismo num. threads secundarios que de posiciones
    - cada thread secudario incrementara 10000 veces el contenido una posicion del vector */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/* Constantes */
#define length 10

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);

long array[length]={0};

void* func(void* arg)
{
    int i;
  
    for (i=0; i<10000; i++)
        *((long int *)arg)+=1; /* Incrementamos el valor de la posicion del array*/
    
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char buffer[128];
    
    pthread_t id [length];
    void * endcode;
    
    int j;
    
    for (j=0; j<length; j++)
        pthread_create(&id[j], NULL, func, (void *)&array[j]); /*Adreza de la posicion del array*/
    /* No podemos pasar directamente &j pq este va cambiando y el SO decide cuando quiere ejecutar el thread*/
        
    for (j=0; j<length; j++)
        pthread_join(id[j],(void **)&endcode);

    for (j=0; j<length; j++)
    {
        sprintf(buffer, "[%ld]", array[j]);
        write(1, buffer, strlen(buffer));    
    } 
  write(1, "\nAcabado\n", 9); 
  return (EXIT_SUCCESS);
}

/* Todos tienen 10000 de resultado*/
