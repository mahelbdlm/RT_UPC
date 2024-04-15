
/*Sesion 2: procesos*/
/*Ej1: Crear hijo y mustrar PID de padre e hijo desde padre y desde hijo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int pid_padre, pid_hijo, ret;

  char buffer[128];

  ret=fork();

  /*En caso que el hijo se ejecute*/  
  if (ret==0){
    pid_hijo=getpid();
    pid_padre=getppid();
    sprintf(buffer, "Soy el hijo.  Mi padre tiene PID: %d. Yo tengo PID: %d.\n",  pid_padre, pid_hijo);
  } 
  /*En caso que el padre se ejecute*/  
  else{
    pid_padre=getpid();
    sprintf(buffer, "Soy el padre. Mi hijo tiene PID: %d. Yo tengo PID: %d.\n", ret, pid_padre);
  }

  write(1,buffer,strlen(buffer));
  return (EXIT_SUCCESS);
}