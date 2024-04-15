/*Sesion 4: Memoria compartida*/
/*Ej4a: Envio de numeros del programa ej4a al ej4b
Protocolo:
    - ej4a puede escribir si ready=1: escribe numero y asigna valid=1
    - ej4b puede leer si valid=1: asigna ready=1, lee el numero y asigna ready=0 
    region[0] = 0;   Number 
    region[1] = 1;   Ready 
    region[2] = 0;   Valid  
*/
/*Library para sprint*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Library para threads*/
#include <pthread.h>

/* Library para ficheros*/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

/*Declaracion del write*/
ssize_t write(int fildes, const void *buf, size_t nbyte);
char buffer[128]; /* Auxiliar para el write*/

/*Constantes*/
#define size 4096

int main(int argc, char *argv[])
{   
    rand(); /* 0 - 2*10^6*/
    int j,i;

    /*-------------------------------------------------*/
    /* Create the shared memory*/
    /*-------------------------------------------------*/
    int r = shm_open("ej4mem", O_CREAT | O_RDWR,0777);
    ftruncate(r, size); // Set region s size
    int *region = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, r, 0);
    close(r); // file descriptor may be closed now

    region[0] = 0; /*  Number */
    region[1] = 1; /*  Ready  */
    region[2] = 0; /*  Valid  */
    region[3] = 1; /*  Running  */

    /*-------------------------------------------------*/
    /* Transmision*/
    /*-------------------------------------------------*/
    for(i=0; i<10; i++) /*Provamos con 100 numeros*/
    {
        while(region[1]!=1); /* Si ready=1*/
        region[0] = rand()%100; /*Asignamos el valor*/
        sprintf(buffer, "Send %d\n", region[0]);
        write(1, buffer, strlen(buffer));
        region[2] = 1; /* valid=0*/
        region[1]=0; /* ready=0*/
    }
    region[3] = 0; /*  Running =0 */

    munmap(region, size);
    shm_unlink("ej4mem");

    sprintf(buffer, "Acabado\n");
    write(1, buffer, strlen(buffer));
    return (EXIT_SUCCESS);
}

/* Caso en que este programa puede fallar:
    - 
*/