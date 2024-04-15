
/*Sesion 2: procesos*/
/*Ej3: Creamos 1000 hijos que se ejecutan sequencialmente:
    - mostraran su Pid
    - el numero de hijo que son*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para procesos*/
#include <sys/types.h>
#include <unistd.h>

/* Library para wait*/
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  int ret, i, j, pid;
  char buffer[128];

  for (i = 0; i < 10; i++)
  {
    ret = fork();
    if (ret == 0)
    {
      pid = getpid();
      sprintf(buffer, "Soy el hijo %d y tengo PID:%d \n", i, pid);
      for (j = 0; j < 1000; j++)
      {
        write(1, buffer, strlen(buffer));
      }
      exit(0);
    }
    wait(NULL);
  }

  return (EXIT_SUCCESS);
}
