
/*Sesion 2: procesos*/
/*Ej2: Con una variable general:
    - decrementar monotonamente con el padre 
    - incrementar monotonamente con el hijo*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para procesos*/
#include <sys/types.h>
#include <unistd.h> 

int main(int argc, char *argv[])
{
  long a=0;
  int ret,i;
  char buffer[128];

  ret=fork();

  /*En caso que el hijo se ejecute*/  
  if (ret==0){
    for (i=0; i<100; i++){
      a++;
      sprintf(buffer,"Hijo:  a=%ld\n", a);
      write(1,buffer,strlen(buffer));
    }
  } 
  /*En caso que el padre se ejecute*/  
  else{
    for (i=0; i<100; i++){    
      a--;
      sprintf(buffer,"Padre: a=%ld\n", a);
      write(1,buffer,strlen(buffer));
    }
  }
  
  return (EXIT_SUCCESS);
}

/*
  Para evitar possible errores, se ha redactado este comentario sin accentos. 

  Observamos que primero se ejecuta el proceso padre, y luego el proceso hijo. 
  Como se copia el valor de a, el proceso padre decrementara la variable a hasta -100
  Idem para el hijo que incrementara la variable a hasta +100.
*/