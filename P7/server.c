/*Servidor*/
/*Descripcion: Calcula las estadisticas que los clientes le envian:
  - numero de ficheros procesados
  - numero de caracteres procesados
  - numero de linesas procesadas
  Si enviamos las senales:
  - SIGUSR1: Se muestra por pantalla las estadisticas
  - SIGUSR2: Se reinician las estadisticas
  - SIGKILL: Lo mata*/
/*Consideraciones:
- Cliente y servidor deben connectarse al mismo puerto
- Protocolo TCP*/

/*Ejemplo ejecución: 
 ./server 3491
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

/*Libreria para los semaforos*/
#include <semaphore.h>

/*Libreria para pthread y los mutex*/
#include <pthread.h>

/*No hacen falta en principio*/
#include <sys/wait.h>
#include <signal.h>

#define PROTOCOL "tcp"  /*Usamos protocolo tcp*/
#define BACKLOG 10      /*Numero de connexiones a la espera*/
#define MAXDATASIZE 128 /*Mida maxima de los buffers de palabras*/

#define NB_THREADS 10   /*Numero de threads que procesan las conexiones*/

sem_t s_padre, s_thread[NB_THREADS]; /* Semaforos que regulan proceso padre y hijo(s)*/
pthread_mutex_t m_threadReturn, m_ficheros, m_car; /*Mutex para proteger variables criticas*/
int threadReturn; /*Contendra el numero del thread que ha acabado y esperando una conexion*/
int sockfd;
int acceptId[NB_THREADS]; /*Vector que contiene los int devueltos por el accept*/

int nbFicheros=0; /*Cuenta el numero de ficheros procesados*/
int nbCaracteres=0; /*Cuenta el numero de caracteres procesados*/
int nbLineas=0; /*Cuenta el numero de lineas procesados*/
/*----------------------------------------------------------------------------------------------*/

void handle_usr(int s){
  pthread_mutex_lock(&m_car);
  char buffer[128];
  if(s==SIGUSR1){
    /*Recibimos SIGUSR1: mostramos las estadisticas*/
    sprintf(buffer, "Numero ficheros procesados: %d\nNumero caracteres procesados: %d\nNumero lineas procesadas: %d\n", nbFicheros, nbCaracteres, nbLineas);
    write(1, buffer, strlen(buffer));
  }
  else{
    /*Recibimos SIGUSR2. Reiniciamos las estadisticas*/
    write(1, "Estadisticas reiniciadas\n", 25);
    nbFicheros=0;
    nbCaracteres=0;
    nbLineas=0;
  }
  pthread_mutex_unlock(&m_car);
}

void *readclient(void *arg)
{
  long numThread = *((long *)arg); /*Numero del thread (1o en crearse, 2o... etc hasta NB_THREADS)*/
  int numbytes; /*Contiene numero de bytes recibidos*/
  char buf[MAXDATASIZE]; /*Buffer para recibir datos*/
  while (1)
  {
    sem_wait(&s_thread[numThread]); /*Espera a desbloqueo de parte del padre*/
    /*Los threads se inicializan con su acceptId correspondiente =-2 */
    if (acceptId[numThread] > 0)
    {
      write(1, "Received connection\n", 20);
      /*Si acceptId tiene un valor >0, es resultado de la crida al accept(). Lo tratamos*/
      pthread_mutex_lock(&m_ficheros);
      nbFicheros++;
      pthread_mutex_unlock(&m_ficheros);

      /*Leemos los datos que se han enviado linea por linea*/
      while ((numbytes = read(acceptId[numThread], buf, MAXDATASIZE - 1)) > 0)
      {
        int numLines = 0, i; /*numLines debe ser 1 porque la ultima linea no tiene \n*/
        for(i=0; i<numbytes; i++){
          if(buf[i]=='\n'){
            numLines++;
          }
        }
        pthread_mutex_lock(&m_car);
        nbCaracteres+=strlen(buf);
        nbLineas+=numLines;
        pthread_mutex_unlock(&m_car);
        for(i=0; i<MAXDATASIZE; i++) buf[i] = '\0';
      }
      pthread_mutex_lock(&m_car);
      nbLineas++; /*La ultima linea no tiene \n*/
      pthread_mutex_unlock(&m_car);
      close(acceptId[numThread]); /*Cerramos conexion*/
    }

    pthread_mutex_lock(&m_threadReturn); /*Se bloquea si otro thread esta en proceso de asignacion. Si no, continua*/
    threadReturn = numThread; /*Contiene el numero del thread acabado*/
    sem_post(&s_padre); /*Desbloquea el padre para que le asigne nueva tarea*/
  }
}

/*----------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  if(argc!=2){
    write(1, "usage: ./server port\n", 21);
    return(EXIT_FAILURE);
  }
  int new_fd; /* Escuchamos en el sock_fd y para nuevas connexiones, lo trataremos con new_fd */
  struct addrinfo hints, *servinfo, *p; /*Para utilizar getaddrinfo*/
  struct sockaddr_storage their_addr; /*informacion de la direccion que recibimos*/
  socklen_t sin_size; /*Longitud de la direccion del cliente*/
  struct sigaction hand, sig2; /*Para inicializar los handlers de signals*/
  sigset_t mask; /*Mascara para signals*/
  pthread_t threadID[NB_THREADS];
  struct protoent *protocol;
  int yes = 1;
  int rv;
  int i; /* Para usar en el for*/
  char buffer[MAXDATASIZE];

  sem_init(&s_padre, 0, 0);
  pthread_mutex_init(&m_threadReturn, NULL);
  pthread_mutex_init(&m_ficheros, NULL);
  pthread_mutex_init(&m_car, NULL);

  sigemptyset(&mask);
  hand.sa_mask = mask;
  hand.sa_flags = 0;
  hand.sa_handler = handle_usr;
  sigaction(SIGUSR1, &hand, NULL);
  sigaction(SIGUSR2, &hand, NULL);

  sprintf(buffer, "PID: %d\n", getpid());
  write(1, buffer, strlen(buffer));

  for (i = 0; i < NB_THREADS; i++)
  {
    sem_init(&s_thread[i], 0, 0);
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
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = protocol->p_proto;

  if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  /* loop de todos los resultados y bind del primero que podamos*/
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }

  freeaddrinfo(servinfo);

  if (p == NULL)
  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1)
  {
    perror("listen");
    exit(1);
  }

  write(1, "server: waiting for connections...\n", 36);

  long j, idThread[NB_THREADS]; /*Vector de int para identificar los threads*/

  /*Creamos los threads con acceptId=2. De esta manera directamente esperaran asignacion de accept()*/
  for (j = 0; j < NB_THREADS; j++)
  {
    idThread[j] = j;
    acceptId[j] = -2;
    pthread_create(&threadID[j], NULL, readclient, (void *)&idThread[j]);
    sem_post(&s_thread[j]); /*Activamos el thread*/
  }

  while (1)
  {
    sem_wait(&s_padre); /*Esperamos a que un thread haya acabado*/
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    acceptId[threadReturn] = new_fd; /*Definimos nuevo acceptId para el thread*/
    sem_post(&s_thread[threadReturn]); /*Desbloqueamos thread*/
    pthread_mutex_unlock(&m_threadReturn); /*Desbloqueamos variable threadReturn*/
  }

  return (EXIT_SUCCESS);
}