/*Cliente*/
/*Descripcion: Envia ficheros a un servidor*/

/* Consideraciones: 
  - Configuracion por defecto como localhost
  - Hemos usado ficheros (-lrt)
*/

/*Ejemplo de sintaxis (con nombres de ficheros) para ejecutar el cliente:

 ./client 3491 ariana birdistheword bowie cardy concalma dojo dojomojocasahouse gaga jennyfer katy lushlife michael rihanna shakira shrek

Esta ejecucion enviara palabras de 15 canciones (en formato txt, en la carpeta txt) al servidor
*/
/*----------------------------------------------------------------------------------------------*/

/*Librerias standard*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Libreria para string*/
#include <string.h>

/*Para control errores*/
#include <errno.h>

/*Libreria para el socket*/
#include <sys/socket.h>
#include <sys/types.h>

/*Libreria para utilizar INET*/
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*No hacen falta en principio*/
#include <sys/wait.h>
#include <signal.h>

/*Libreria para utilizar open()*/
#include <fcntl.h>

/*Libreria para gettimeofday*/
/*El primer parametro de gettimofday guarda s y us y el segundo los minutos y la data*/
#include <sys/time.h>

/* Library para ficheros*/
#include <sys/mman.h>
#include <sys/stat.h>

#define SERVER "localhost" /*IP of server (localhost = 127.0.0.1 (ip interna) )*/
#define PROTOCOL "tcp"     /*Usamos protocolo tcp*/

#define size 4096 /*Mida maxima para el fichero que guarda los bytes enviados*/

#define MAXDATASIZE 128 /*Mida maxima de los buffers de palabras*/

/*----------------------------------------------------------------------------------------------*/
/*Para conseguir la adresa en IPv4 o IPv6 */
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*----------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int sockfd, numbytes, i, rv;

  char buffer[MAXDATASIZE];
  struct protoent *protocol;        /*Para obtener el numero de protocolo de #define PROTOCOL*/
  struct addrinfo hints, *servinfo; /*Para poder usar getaddrinfo*/
  
  /* Estructuras y variables para guardar la diferencia de tiempos para enviar un fichero*/
  struct timeval ta, tb; 
  long int usec=0;
  long int sec=0;
  long int min=0;

  /*Usamos un fichero para almacenar cuantos Bytes se envian de cada fichero para que el padre muestre el total por pantalla*/
  int r = shm_open("fichero_bytes", O_CREAT | O_RDWR,0777);
  if (r==-1) perror("No pudo abrir 'fichero_bytes' ");
  ftruncate(r, size); 
  int *region = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, r, 0);
  close(r); 
   
  /*Verificamos que nos pasen ficheros como parametros*/
  if (argc < 3)
  {
    write(1, "usage: ./client port file [list_of_files]\n", 42);
    return (EXIT_FAILURE);
  }
  int fp[argc - 2]; /*Creamos punteros de texto para poder leer*/

  /*Obtenemos el struc protocolo a partir de la constante */
  protocol = getprotobyname(PROTOCOL);
  if (protocol == NULL)
  {
    write(1, "Could not get protocol\n", 23);
    exit(EXIT_FAILURE);
  }
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = protocol->p_proto;

  if ((rv = getaddrinfo(SERVER, argv[1], &hints, &servinfo)) != 0)
  {
    sprintf(buffer, "getaddrinfo: %s\n", gai_strerror(rv));
    write(1, buffer, strlen(buffer));
    return (EXIT_FAILURE);
  }

  /*Loop para cada fichero */
  
  gettimeofday(&ta, NULL); /*Empezamos a contar el tiempo de transmision*/

  for (i = 2; i < argc; i++)
  {
    if (!fork())
    {
      /*hijo*/
      char s[INET6_ADDRSTRLEN];      /*Buffer para guardar el client IP*/
      char fileName[20];             /*Guardar file name*/
      char fileContent[MAXDATASIZE]; /*Para guardar el contenido del fichero*/
      int numBytesFile=0; /*Numero total de bytes del fichero*/
      sprintf(fileName, "./txt/%s.txt", argv[i]);
      fp[i] = open(fileName, O_RDONLY); /*Abrimos el fichero en read only*/
      if (fp[i] == -1)
      {
        /*Error al abrir el fichero*/
        sprintf(buffer, "An error happened while trying to open file %s\n", fileName);
        write(1, buffer, strlen(buffer));
        exit(EXIT_FAILURE);
      }

      /*El fichero se ha abierto correctament. Creamos el socket y lo connectamos al cliente*/
      if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
      {
        perror("client: socket");
      }

      if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
      {
        close(sockfd);
        perror("client: connect");
      }

      if (servinfo == NULL)
      {
        write(1, "client: failed to connect\n", 26);
        return(EXIT_FAILURE);
      }

      /*Conseguimos la adressa IP del Client*/
      inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr), s, sizeof s);
      printf("client: connecting to %s\n", s);
      freeaddrinfo(servinfo);

      /*Recorremos cada linea del txt file y la enviamos al servidor*/
      while (read(fp[i], fileContent, MAXDATASIZE-1)>0)
      {
        numBytesFile+=strlen(fileContent);
        if (write(sockfd, fileContent, MAXDATASIZE-1) == -1)
          perror("send error: ");
        int j;
        for(j=0; j<MAXDATASIZE; j++)fileContent[j]='\0';
      }

      if (close(fp[i]))
        perror("close error: ");
      
      /*El mensaje se ha enviado correctamente. Cerramos el socket*/
      close(sockfd);
      region[i]=numBytesFile;
      
      exit(EXIT_SUCCESS); /*Si no ha habido ningun error, se habra enviado numbytes File*/
    }
  }
  /*Esperamos que terminen todos los procesos y analisamos los bytes enviados los cuales se indican en el status*/
  while (waitpid(-1, NULL, 0) > 0);

  numbytes=0;
  for (i = 2; i < argc; i++){
    numbytes+=region[i];
  }
  munmap(region,size);
  shm_unlink("fichero_bytes");
  sprintf(buffer, "Numero bytes enviados: %d\n", numbytes);
  write(1, buffer, strlen(buffer));

  /*Analisis del tiempo de envio*/
  gettimeofday(&tb,NULL);

  if (tb.tv_sec-ta.tv_sec>=0){
    usec=tb.tv_usec-ta.tv_usec;
    if(usec<0) usec=ta.tv_usec;
    sec=(tb.tv_sec-ta.tv_sec)%60;
    min=(tb.tv_sec-ta.tv_sec)/60;

    sprintf(buffer, "Tiempo para enviar los ficheros: %ld min, %ld sec, %ld usec\n", min, sec,usec );
    write(1, buffer, strlen(buffer));
  }
  else{
    sprintf(buffer, "Tiempo no medido\n");
    write(1, buffer, strlen(buffer));
  }

  return (EXIT_SUCCESS);
}

