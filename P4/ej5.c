/*Sesion 4: Memoria compartida*/
/*Ej3: Simulacion de la cena de los filosofos
    - thread=filosfo (5)
    - tenedores=vector de posiciones
La solucion que ofrecemos segun las visto en clase es la de coger todos los recursos necesarios o ni uno,
de esta forma eliminamos el sistema "Hold and Wait"*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t tenedor[5];
int ind[5];
int rechazos[5] = {0};

void comer(void)
{
  int i = 0;
  while (i < 10000)
    i++;
}

void *filosofo(void *arg)
{
  char buffer[100];
  int filo = *((int *)arg);
  int filo2 = ((filo) + 1) % 5;

  int lock1 = pthread_mutex_trylock(&tenedor[filo]);  /*Filosofo intenta coger su tenedor*/
  int lock2 = pthread_mutex_trylock(&tenedor[filo2]); /* Filosofo intenta coger el tenedor del otro lado*/

  while (1)
  {
    while (lock1 != 0 || lock2 != 0)
    {
      /*Uno o los dos tenedores no estan disponibles*/
      if (lock1 == 0)
        pthread_mutex_unlock(&tenedor[filo]); /*Libera los tenedores*/
      if (lock2 == 0)
        pthread_mutex_unlock(&tenedor[filo2]);        /* Libera los tenedores*/
      lock1 = pthread_mutex_trylock(&tenedor[filo]);  /*Filosofo intenta coger su tenedor*/
      lock2 = pthread_mutex_trylock(&tenedor[filo2]); /* Filosofo intenta coger el tenedor del otro lado*/
      rechazos[filo]++;                               /*Incrementamos los intentos del filosofo */
    }
    comer(); /*Ya puede comer*/
    sprintf(buffer, "Filosofo %d esta comiendo\n", filo);
    write(1, buffer, strlen(buffer));
    pthread_mutex_unlock(&tenedor[filo]);
    pthread_mutex_unlock(&tenedor[filo2]);
  }
}

int main(int argc, char **argv)
{
  int termcode, i, j;
  char buffer[100];
  pthread_t id[5];
  for (i = 0; i < 5; i++)
  {
    /* Inicializacion del mutex*/
    pthread_mutex_init(&tenedor[i], NULL);
  }
  for (i = 0; i < 5; i++)
  {
    ind[i] = i; /*Variable que guarda el indice del filosofo*/
    pthread_create(&id[i], NULL, filosofo, (void *)&ind[i]);
  }
  for (i = 0; i < 5; i++)
  {
    pthread_join(id[i], (void **)&termcode);
  }

  sprintf(buffer, "Cena acabada...Intentos de los filosofos: \n");
  write(1, buffer, strlen(buffer));
  sprintf(buffer, "[%d", rechazos[0]);
  write(1, buffer, strlen(buffer));
  for (i = 1; i < 5; i++)
  {
    sprintf(buffer, ", %d", rechazos[i]);
    write(1, buffer, strlen(buffer));
  }
  write(1, "]\n", 2);
  return (EXIT_SUCCESS);
}

/*
Se muestra al final de la ejecucion un vector con los intentos de trylock de los filosofos.
Unos tienen mas suerte que otros...
*/