/*Sesion 6: Memoria Distribuida*/
/*Ej2a: Envio de numeros del programa ej2a al ej2b usando named pipe
 */
/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Library para pipe*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "/tmp/myfifo"

/*Declaracion del write*/
char buffer[128]; /* Auxiliar para el write*/

int main(int argc, char *argv[])
{
  int i;
  char buffer[128];
  if(argc!=2){
    write(1, "Especifica una frase a enviar...\n", 33);
    return(EXIT_FAILURE);
  }

  rand(); /* 0 - 2*10^6*/

  mkfifo(FIFO_NAME, S_IRUSR|S_IWUSR);
  int fd = open(FIFO_NAME, O_WRONLY);

  if (fd ==-1) {
    write(1, "Error\n",6);
    return(EXIT_FAILURE);
  }
  /* Transmision*/
  for(i=0; i<1000; i++)/*Provamos con 100 numeros*/
  {
    int randNum = rand()%100;
    sprintf(buffer, "%s:%d\n", argv[1], randNum);
    write(fd, buffer, strlen(buffer));    /*2ndo enviamos el numero*/

  /*Lo mostramos por la pantalla del terminal*/
    sprintf(buffer, "Send %d\n", randNum);
    write(1, buffer, strlen(buffer));
  }

  close(fd);

  return (EXIT_SUCCESS);
}
