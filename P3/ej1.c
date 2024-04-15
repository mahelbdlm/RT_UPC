/*Sesion 3: threads*/
/*Ej1: Crear thread que salude y muera*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);

void* func(void* arg)
{
  write(1,"Hola\n",5);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  char buffer[128];
  
  pthread_t id;
  void * endcode;
  pthread_create(&id, NULL, func, NULL);
  pthread_join(id,(void **)&endcode);
  write(1, "Acabado\n", 8);     
  
  return (EXIT_SUCCESS);
}
