/*Sesion 6: Memoria Distribuida*/
/*Ej1a: Envio de numeros del programa ej1a al ej1b usando named pipe
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

int main(int argc, char *argv[])
{
  rand(); /* 0 - 2*10^6*/
  int i;
  char buffer[128];

  mkfifo(FIFO_NAME, S_IRUSR|S_IWUSR);
  int fd = open(FIFO_NAME, O_WRONLY);

  if (fd ==-1) {
    write(1, "Error\n",6);
    return(EXIT_FAILURE);
  }
  /* Transmision*/
  for(i=0; i<10; i++)/*Provamos con 100 numeros*/
  {
    int randNum = rand() % 100;
    int2str(randNum, buffer);
    write(fd, buffer, strlen(buffer));

  /*Lo mostramos por la pantalla del terminal*/
    sprintf(buffer, "Send %d\n", randNum);
    write(1, buffer, strlen(buffer));
  }

  close(fd);
  

  return (EXIT_SUCCESS);
}
