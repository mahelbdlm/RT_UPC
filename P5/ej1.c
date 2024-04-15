
/*Sesion 4: Memoria compartida*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void int_handler(int s) {
  char buffer[128];
  sprintf(buffer, "SIGNAL RECIBIDO: %d\n", s);
  write(0, buffer, strlen(buffer));
  /*exit(0);*/
  /*No debemos llamar a exit sino se muere el proceso...*/
}

int main(int argc, char **argv)
{
  char buffer[128];
  int i;
  struct sigaction hand;
  sigset_t mask;
  sigemptyset(&mask);
  hand.sa_mask = mask;
  hand.sa_flags = 0;
  hand.sa_handler = int_handler;
  for(i=0; i<32; i++){
    sigaction(i,&hand,NULL);
  }
  sprintf(buffer, "PID: %d\n", getpid());
  write(0, buffer, strlen(buffer));
  while(1);
  return (EXIT_SUCCESS);
}