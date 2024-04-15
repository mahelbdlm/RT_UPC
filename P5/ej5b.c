/*Sesion 5: Signal*/
/*Ej5a:  Medicion de los us que le pasemos como parametro usando espera activa*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para gettimeofday*/
/*El primer parametre de gettimeofday guarda los s y us
    El segundo, minutos y data con lo que no nos interesa en este caso */
#include <sys/time.h>

/*Structs timeval:
    - sus atributos son tv_sec, tv_usec que son long int*/
struct timeval ta, tb;

int str2num(char *str)
{
  int length = strlen(str), i, num = 0;
  char nbHex[length + 1];
  for (i = 0; i < length; i++)
  {
    num = num * 10;
    num += str[i] - '0';
  }
  return num;
}

int main(int argc, char *argv[])
{
  char buffer[128];
  if (argc != 2)
  {
    /*No params were given. Show error*/
    write(1, "Debes especificar un parametro: [tiempo en us]\n", 47);
    return EXIT_FAILURE;
  }

  gettimeofday(&ta, NULL);
  while (((tb.tv_sec * 1000000 + tb.tv_usec) - (ta.tv_sec * 1000000 + ta.tv_usec)) < str2num(argv[1]))
    gettimeofday(&tb, NULL);
  /*Lo expresamos todo en us*/

  sprintf(buffer, "Tiempo transcurrido: %ld [us]\n", (tb.tv_sec * 1000000 + tb.tv_usec) - (ta.tv_sec * 1000000 + ta.tv_usec));
  write(1, buffer, strlen(buffer));

  return (EXIT_SUCCESS);
}

/* Observamos que los tiempos son practicamente exactos*/
