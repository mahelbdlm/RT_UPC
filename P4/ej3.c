/*Sesion 4: Memoria compartida*/
/*Ej3: Creamos 10 threads paralelos
    - los pares tiene que mostrar su id de forma creciente
    - los impares de forma decreciente */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/* Library para semaforos*/
#include <semaphore.h>

/*Numero de threads*/
#define length 10

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);
char buffer[128]; /* Auxiliar para el write*/

/* Estructuras para semaforos*/
sem_t semaforos[length];
int ind[10]; /*Guarda los indices del semaforo*/

void* func(void* arg)
{   
    sem_wait(&semaforos[*((int*) arg)]);
    sprintf(buffer, "Num: %d\n", *((int*) arg));
    write(0, buffer, strlen(buffer));

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{    
    int i;
    pthread_t id[length];
    void * endcode;

    for(i=0; i<length; i++){
      sem_init(&semaforos[i], 0, 0);
    }
    for(i=0; i<length;i++){
      ind[i] = i;
      pthread_create (&id[i], NULL, func, (void *)&ind[i]);
      /*Creamos procesos paralelos que activaremos con los semaforos*/
    }
    for(i=0; i<length; i+=2){
      sem_post(&semaforos[i]);
      pthread_join(id[i],(void**)&endcode);
    }
    for(i=9; i>0; i-=2){
      sem_post(&semaforos[i]);
      pthread_join(id[i],(void**)&endcode);
    }

    sprintf(buffer, "Acabado\n");
    write(1, buffer, strlen(buffer));

  return (EXIT_SUCCESS);
}