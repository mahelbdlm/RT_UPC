/*Sesion 6: Memoria Distribuida*/
/*Ej2b: Envio de numeros del programa ej4a al ej4b usando named pipe
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
  int fd = open(FIFO_NAME, O_RDONLY); 
  
  if (fd ==-1) {
    write(1, "Error\n",6);
    return(EXIT_FAILURE);
  }
  write(1, "Recibimos:\n", 11);
  while(read(fd, buffer, sizeof(buffer))!=0){
    write(1, buffer, strlen(buffer));
  }

  close(fd);

  return (EXIT_SUCCESS);
}

/*Observamos el principal problema de las named pipes: 
    - como la pipe tienen poca capacidad, se pierden mensajes*/