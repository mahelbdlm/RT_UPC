/*Sesion 5: Signal*/
/*Ej4:  - Padre creara N hijos como el parametro que le pasemos
        - Padre suspendera su ejecucion hasta la muerte de un hijo que envie la senyal SIGCHILD:
            + Mostrar el pid + resultado del hijo finalizado */

#include <unistd.h>

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para signal*/
#include <signal.h>

/*Library para procesos*/
#include <sys/types.h>
#include <sys/wait.h>

int str2num(char *str)
{
  int length = strlen(str), i, num = 0;
  char nbHex[length + 1];
  for (i = 0; i < length; i++)
  {
    num = num * 10;
    num += str[i] - '0';
  }
  return num;
}

void int_handler(int s)
{
    char buffer[128];
    int exit_code;
    int pid;
    pid=waitpid(-1,&exit_code, 0); 
    sprintf(buffer, "Hijo finalizado con PID[%d], tiene como resultado: %d\n",pid, exit_code);
    write(1, buffer, strlen(buffer)); 
}
int main(int argc, char *argv[])
{
  int i, ret;
  struct sigaction hand;
  sigset_t mask;
  if (argc == 1)
  {
    /*No params were given. Show error*/
    write(1, "Debes especificar un parametro [nombre de hijos a crear]\n", 31);
    return EXIT_FAILURE;
  }

  /*Evitamos condiciones de carrera*/
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);
  sigprocmask(SIG_BLOCK, &mask, NULL);
  
  sigemptyset(&mask);
  hand.sa_handler= int_handler;
  hand.sa_mask = mask;
  hand.sa_flags = 0;
  sigaction(SIGCHLD, &hand, NULL); 

  /*Mascara para el sigsuspend*/
  sigfillset(&mask);
  sigdelset(&mask, SIGCHLD);

  for (i=0; i<str2num(argv[1]); i++)
  {
    ret=fork();
    if (ret==0){
      exit(0);
    } 
    else{
      sigsuspend(&mask);
    }
  }  
}
