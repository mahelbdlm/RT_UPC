#include "actuators.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> /*for usleep*/
int pump1_fd, pump2_fd, drift_fd;
int init_actuators()
{
  int pump1_stat = connect_socket(20000, &pump1_fd);
  int pump2_stat = connect_socket(20001, &pump2_fd);
  int drift_stat = connect_socket(20002, &drift_fd);

  return 0;
}

int read_pipe_active_float(int fd, float *value)
{
  read(fd, value, sizeof(float));
  return 0;
}

static int connect_socket(int port, int *fd)
{
  struct sockaddr_in serv_addr;
  int try;
  if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }
  try = 0;
  while ((connect(*fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
          0) &&
         (try < 5))
  {
    try++;
    usleep(500000);
  }
  if (try == 5)
    return -1;
  return 0;
}
