
/*Sesion 4: Memoria compartida*/
/*Ejercicio 2_A: Utilizando mutex_lock*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
pthread_mutex_t tenedor[5];
int ind[5];

void comer(void)
{
  int i = 0;
  while (i < 1000)
    i++;
}

void *filosofo(void *arg)
{
  char buffer[100];
  int filo = *((int *)arg);
  int filo2 = (5 + (filo - 1)) % 5;
  /*Si uno de los tenedores siguientes no esta disponible, se espera a que lo este...*/

  while (1)
  {
    int lock1 = pthread_mutex_lock(&tenedor[filo]);  /*Coge un tenedor*/
    int lock2 = pthread_mutex_lock(&tenedor[filo2]); /*Coge el otro tenedor*/
    comer();
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
    pthread_mutex_init(&tenedor[i], NULL);
  }
  for (i = 0; i < 5; i++)
  {
    ind[i] = i;
    pthread_create(&id[i], NULL, filosofo, (void *)&ind[i]);
  }
  for (i = 0; i < 5; i++)
  {
    pthread_join(id[i], (void **)&termcode);
  }

  sprintf(buffer, "Cena acabada...\n");
  write(1, buffer, strlen(buffer));
  return (EXIT_SUCCESS);
}

/*
En este ejercicio, solo utilizamos lock.
Los filosofos pueden comer, pero deben esperar su turno.

En nuestras simulaciones, siempre habia un thread que cogia 2 tenedores a la vez
con lo que todos terminaban comiendo.
Pero podria haber la posibilidad de que todos cogieran solo 1 tenedor a la vez, entonces deadlock
Con lock, nunca hay rechazos pq se espera hasta que le den el recurso, no lo libera

En el ej2_b.c, utilizamos trylock y contamos el numero de intentos
*/