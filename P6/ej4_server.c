/*Sesion 6: Memoria Distribuida*/
/* 
Ej4: Server de un sockets: Recibe numeros de un cliente
  - Protocolo TCP
  - Tipo de conneccion: stream
  - addres family: inet
*/
/* Nota: Cliente y servidor deben connectarse a la misma IP*/

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
#define BACKLOG 10   /*Numero de connexiones a la espera*/

/*Trataremos los errores con errno*/
void sigchld_handler(int s)
{
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/* Conseguimos sockaddr, IPv4 or IPv6: */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
  int sockfd, new_fd;  /* Escuchamos en el sock_fd y para nuevas connexiones, lo trataremos con new_fd */
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; /*informacion de la adresa que recivimos*/
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  char buffer[128]; /*Buffer para guardar los mensajes a enviar*/

  int i; /* Para usar en el for*/

  struct protoent *protocol;

  protocol = getprotobyname(PROTOCOL);
  if (protocol == NULL)
  {
    write(1, "Could not get protocol\n", 23);
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; 
  hints.ai_protocol=protocol->p_proto;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
  }

  /* loop de todos los resultados y bind del primero que podamos*/ 
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
          perror("server: socket");
          continue;
      }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
          perror("setsockopt");
          exit(1);
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          perror("server: bind");
          continue;
      }
      break;
  }

  freeaddrinfo(servinfo); 

  if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
      perror("listen");
      exit(1);
  }

  sa.sa_handler = sigchld_handler; /*Recogemos los procesos muertos */
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
  }

  printf("server: waiting for connections...\n");

  while(1) {  /* accept() loop*/ 
      sin_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (new_fd == -1) {
          perror("accept");
          continue;
      }

      inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
      printf("server: got connection from %s\n", s);

      if (!fork()) { /*hijo*/
        close(sockfd); /* El hijo no necesita listener*/

        //for (i = 0; i < 30; i++) /*Provamos con 100 numeros*/
        while(1)
        {
            int randNum = rand() % 100;
            /*Lo mostramos por la pantalla del terminal*/
            sprintf(buffer, "%d\n", randNum);
            write(1, buffer, strlen(buffer));

            if (write(new_fd, buffer, strlen(buffer)) == -1)
                perror("send");
        }
          close(new_fd);
          exit(0);
      }
      close(new_fd);  /*Padre no lo necesista*/
  }

  return 0;
}
