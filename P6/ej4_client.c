/*Sesion 6: Memoria Distribuida*/
/* Nota: Basado en Beej's Guide to Network programming
Ej4: Cliente de un sockets: Envia numeros a un servidor
  - Protocolo TCP
  - Tipo de conneccion: stream
  - addres family: inet compatible con IPV4 Y  IPV6 !!! (Lo hemos hecho para los 2)
*/
/* Nota: Cliente y servidor deben connectarse a la misma IP*/
/* Para ejecutarlo en el mismo ordenador: ./ej4_cliente localhost*/
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

#define PORT "3490"    /*Puerta donde se connecta cliente*/
#define PROTOCOL "tcp" /*Usamos protocolo tcp*/

#define MAXDATASIZE 100 // Numero maximo de numeros que podemos conseguir

/*sockaddr, IPv4 or IPv6: */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  struct protoent *protocol;

  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }

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

  if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
  }

  /* loop de todos los resultados y bind del primero que podamos*/
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
          perror("client: socket");
          continue;
      }

      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          perror("client: connect");
          continue;
      }
      break;
  }

  if (p == NULL) {
      fprintf(stderr, "client: failed to connect\n");
      return 2;
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connecting to %s\n", s);

  freeaddrinfo(servinfo); 

  while((numbytes = read(sockfd, buf, MAXDATASIZE-1))>0) {
    buf[numbytes] = '\0';
    printf("%s\n",buf);
  }
  perror("recv");
  exit(1);
  close(sockfd);

  return 0;
}