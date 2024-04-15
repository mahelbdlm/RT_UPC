/*Sesion 5: Signal*/
/*Ej5c_a: Medicion de los N us que le pasemos como 1er-parametro usando espera "usleep"
        A la vez, se ejcutaran M threads: M sera el 2 parametro o, que esperaran N us_medidos*/

/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Library para threads*/
#include <pthread.h>

/*Library para gettimeofday*/
/*El primer parametre de gettimeofday guarda los s y us 
    El segundo, minutos y data con lo que no nos interesa en este caso */

#include <sys/time.h>

/*Structs timeval:
    - sus atributos son tv_sec, tv_us_medidos que son long int*/
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

void * func(void*arg)
{   
  long  int a=*(long int *)arg;
    usleep(a);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char buffer[128];
    int i;
    long int us_medidos=0;
    long int us_pedidos=str2num(argv[1]);

    if (argc != 3)
    {
      write(1, "Parametros invalidos. Debes especificar [tiempo en us] [numero threads]\n", 72);
      return (EXIT_FAILURE);
    }

    pthread_t id[str2num(argv[2])];
    void *endcode;
    
    for(i=0; i<str2num(argv[2]); i++)
        pthread_create(&id[i], NULL, func, (void *)&us_pedidos);

    gettimeofday(&ta,NULL);    
    usleep(us_pedidos);
    gettimeofday(&tb,NULL);

    us_medidos=(tb.tv_sec*1000000+tb.tv_usec)-(ta.tv_sec*1000000+ta.tv_usec);

    for(i=0; i<str2num(argv[2]); i++)
        pthread_join(id[i], (void **)&endcode);

    sprintf(buffer, "Tiempo transcurrido: %ld [us]\n",us_medidos);
    write(1, buffer, strlen(buffer)); 

    sprintf(buffer, "Diferencia de tiempo (Entre real y el exigido): %ld [us]\n",us_medidos-us_pedidos);
    write(1, buffer, strlen(buffer)); 

    return (EXIT_SUCCESS);
}
