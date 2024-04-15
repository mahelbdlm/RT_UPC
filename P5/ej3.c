/*Sesion 5: Signal*/
/*Ej3:  - Mostrar un tiempo transcurrido des del principio cada 2 seg
        - Morira al recibir SIGINT mostrando el numero de mensajes*/
/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para signal*/
#include <signal.h>

/*Globales*/
long int t=0; /*Para almacenar la variable tiempo*/

void int_handler(int s)
{
    char buffer[128]; 
    if (s==SIGALRM)
    {
        t+=2;
        sprintf(buffer, "Mensaje(%ld)- Tiempo transcurrido: %ld [s]\n",t/2,t);
        write(1, buffer, strlen(buffer));  
    }
    if (s==SIGINT)
    {
        t=t/2;
        sprintf(buffer, "Mensajes enviados: %ld\n",t);
        write(1, buffer, strlen(buffer));         
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction hand;
    sigset_t mask;

    /*Para evitar las condiciones de carrera, bloqueamos la senyal SIGALRM*/
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&mask);
    hand.sa_handler= int_handler;
    hand.sa_mask = mask;
    hand.sa_flags = 0;
    sigaction(SIGALRM, &hand, NULL); 
    sigaction(SIGINT, &hand, NULL); 

    /*Mascara para sigsuspend*/
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigdelset(&mask, SIGALRM);
    while(1){
        alarm(2);
        sigsuspend(&mask);
    } 
}