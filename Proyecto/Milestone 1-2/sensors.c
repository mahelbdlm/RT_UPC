#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> /*for usleep*/

int speed_fd_read, speed_fd_write, alt1_fd, alt2_fd, alt3_fd, leftsensor_fd, rightsensor_fd;
int init_sensors()
{
  int speed_stat = connect_pipe("speedpipe", &speed_fd_read, &speed_fd_write);
  int alt1_stat = connect_socket(20010, &alt1_fd);
  int alt2_stat = connect_socket(20011, &alt2_fd);
  int alt3_stat = connect_socket(20012, &alt3_fd);
  int leftsensor_stat = connect_socket(20015, &leftsensor_fd);
  int rightsensor_stat = connect_socket(20016, &rightsensor_fd);
  return 0;
}

static int connect_pipe(char *pipe_name, int *fd_read, int *fd_write)
{
  char channel_read[256];
  char channel_write[256];
  int try;
  sprintf(channel_read, "%s_read", pipe_name);
  sprintf(channel_write, "%s_write", pipe_name);
  try = 0;
  while (((*fd_read = open(channel_read, O_RDONLY)) < 0) && (try < 5))
  {
    usleep(500000);
    try++;
  }
  if (*fd_read < 0)
    return -1;
  try = 0;
  while (((*fd_write = open(channel_write, O_RDWR | O_NONBLOCK)) < 0) && (try < 5))
  {
    usleep(500000);
    try++;
  }
  if (*fd_write < 0)
    return -1;
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
