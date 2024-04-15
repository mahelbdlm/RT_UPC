
/*Sesion 5: Signal*/
/*Ej2:  -Un proceso hijo contara los SIGUSR1 enviados por el padre y cuando muera lo printara
        -El proceso padre envaira 1000 SIGUSR1 y 1 SIGUSR2 y morira */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para procesos*/
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/*Library para signal*/
#include <signal.h>

int numsign=0; /* Variable global que usara el hijo para incrementar*/

void int_handler(int s)
{
    char buffer[128];  
    
    if (s==SIGUSR1){ 
        numsign++;
    } 
    if (s==SIGUSR2)
    {
        sprintf(buffer, "numsign: %d\n", numsign);
        write(1, buffer, strlen(buffer));  
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    char buffer[128];
    struct sigaction hand;
    sigset_t mask;
    int ret,i,j;

    sigemptyset(&mask);
    hand.sa_handler= int_handler;
    hand.sa_mask = mask;
    hand.sa_flags = 0;

    /* Pasamos el mismo sigaction a las 2 senyales*/
    sigaction(SIGUSR1, &hand, NULL); 
    sigaction(SIGUSR2, &hand, NULL);

    /*Creamos el hijo que ejecutara el handler*/
    ret=fork();

    if (ret==0){
        while(1);
    }
    else{
        for (i=0; i<1000; i++)
        {   
            for(j=0; j<100000; j++); 
            kill(ret, SIGUSR1);
        }
        kill(ret, SIGUSR2);
        wait(NULL);
    }

  write(1, "Acabado\n", 9); 
  return (EXIT_SUCCESS);
}

/*El padre enviara las senyales mas paulativamente con la ayuda de un for 
 para que el hijo pueda procesarlos y no pierda tantos*/
 /*De esta forma, recibimos 700 de los 1000 enviados*/