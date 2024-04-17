/*Sesion 6: Memoria Distribuida*/
/*Ej3b: Envio de numeros del programa ej3a al ej3b usando message queues
 */

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para las colas*/
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#define QUEUE_NAME  "/platypus"
#define MAX_SIZE    1024

int main(int argc, char **argv)
{
  mqd_t mq;
  struct mq_attr attr;
  char buffer[MAX_SIZE + 1];
  int must_stop = 0;

  /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
  if(mq==-1){
    write(1, "Error opening queue\n",20);
    return(EXIT_FAILURE);
  }
  ssize_t nbBytes;

  /* receive the message */
  nbBytes = mq_receive(mq, buffer, MAX_SIZE, NULL);
  buffer[nbBytes] = '\0';
  while (strncmp(buffer, "exit", 4) != 0)
  {
    printf("Received: %s\n", buffer);
    nbBytes = mq_receive(mq, buffer, MAX_SIZE, NULL);
    buffer[nbBytes] = '\0';
  }

  /* cleanup */
  if(mq_close(mq)==-1){
    write(1, "Error closing queue\n",20);
    return(EXIT_FAILURE);
  }
  if(mq_unlink(QUEUE_NAME)==-1){
    write(1, "Error unlinking queue\n",22);
    return(EXIT_FAILURE);
  }

  return 0;
}