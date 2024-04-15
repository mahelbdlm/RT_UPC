/*Sesion 3: threads*/
/*Ej2: Variable global
    - proceso decrementa montomamente
    - thread incrementa monotomamente */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);

long a;

void* func(void* arg)
{
    char buffer[128];
    int i;
    for (i=0; i<1000000; i++){
      a++;
      sprintf(buffer,"Thread_hijo:  a=%ld\n", a);
      write(1,buffer,strlen(buffer));
    }
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char buffer[128];
    
    pthread_t id;
    void * endcode;
    pthread_create(&id, NULL, func, NULL);

    int i;
    for (i=0; i<1000000; i++){
        a--;
        sprintf(buffer,"Proceso_padre:  a=%ld\n", a);
        write(1,buffer,strlen(buffer));
    }
    pthread_join(id,(void **)&endcode);
    write(1, "Acabado\n", 8);     
  
  return (EXIT_SUCCESS);
}

/* El orden, lo decide la PCB */
    