/*Sesion 6: Memoria Distribuida*/
/*Ej3a: Envio de numeros del programa ej3a al ej3b usando message queues
 */
/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para las colas*/
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#define QUEUE_NAME  "/platypus"
#define MAX_SIZE    1024

int main(int argc, char **argv)
{
  mqd_t mq;
  char buffer[MAX_SIZE];
  int i;

  if(argc!=2){
    write(1, "Especifica una frase a enviar...\n", 33);
    return(EXIT_FAILURE);
  }

  /* open the mail queue */
  mq = mq_open(QUEUE_NAME, O_WRONLY);
  if(mq==-1){
    write(1, "Error opening queue\n",20);
    return(EXIT_FAILURE);
  }

  for (i = 0; i < 1000; i++) /*Provamos con 100 numeros*/
  {
    int randNum = rand() % 100;
    
    sprintf(buffer, "%s:%d\n", argv[1], randNum);
    mq_send(mq, buffer, strlen(buffer), 0);

    /*Lo mostramos por la pantalla del terminal*/

    sprintf(buffer, "Send %d\n", randNum);
    write(1, buffer, strlen(buffer));

  }
  mq_send(mq, "exit", 5, 0);

  /* cleanup */
  if(mq_close(mq)==-1){
    write(1, "Error closing queue\n",20);
    return(EXIT_FAILURE);
  }

  return 0;
}
