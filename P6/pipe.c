/*Sesion 6: Memoria Distribuida*/
/*Ej1_pipe: Envio de numeros de un proceso a otro usando named pipe
 */
/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int int2str(int num, char* txt)
{
  int i;
  if(num<0){
    return(EXIT_FAILURE);
  }
  int temp = num; /* Stores the number */
  int lgthNum = 0;
  /* Counts the length of the resulting string
  ex: 42 -> length=2; 123 -> length=3 ... */
  while (temp > 0)
  {
    temp = temp / 10;
    lgthNum++;
  }
  temp = num;
  for (i = lgthNum - 1; i >= 0; i--)
  {
    txt[i] = (temp % 10) + '0'; /*Convert number to str (using ascii table) */
    temp = temp / 10;
  }
  txt[lgthNum] = '\n';    
  txt[lgthNum+1] = '\0';   
  return(EXIT_SUCCESS);
}

int main() {
    int pipefd[2]; // file descriptors for the pipe
    char buffer[128];
    pid_t pid;

    int i;

    /*Creamos la pipe*/
    if (pipe(pipefd) == -1) {
        return(EXIT_FAILURE);
    }

    /*Creamos un proceso hijo*/
    pid = fork();
    if (pid == -1) {
        return(EXIT_FAILURE);
    }

    if (pid == 0) { /* child*/ 
        close(pipefd[1]); /*Unidireccional*/ 

        /*Lectura*/
        while(read(pipefd[0], buffer, sizeof(buffer))!=0){
            write(1, "Recibimos:\n", 11);
            write(1, buffer, strlen(buffer));
            write(1, "\n", 1);
        }

        /* Finalizamos el canal de lectura */
        close(pipefd[0]);

    } else { /*Padre*/
        close(pipefd[0]); /*Unidireccional*/ 

        /* Transmision*/
        for(i=0; i<10; i++)/*Provamos con 100 numeros*/
        {
            int randNum = rand() % 100;
            int2str(randNum, buffer);
            write(pipefd[1], buffer, strlen(buffer));

        /*Lo mostramos por la pantalla del terminal*/
            sprintf(buffer, "Send %d\n", randNum);
            write(1, buffer, strlen(buffer));
        }

         /* Finalizamos el canal de eescritura */
        close(pipefd[1]);
    }

    return 0;
}
