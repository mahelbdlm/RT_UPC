#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

int init_sensors()
{
  int speed_fd, alt1_fd, alt2_fd, alt3_fd, leftsensor_fd, rightsensor_fd;
  int speed_stat = connect_pipe("Speedpipe", &speed_fd, NULL); //Speed is a sensor => Only Read (not write)
  int alt1_stat = connect_socket(20010, alt1_fd);
  int alt2_stat = connect_socket(20011, alt2_fd);
  int alt3_stat = connect_socket(20012, alt3_fd);
  int leftsensor_stat = connect_socket(20015, leftsensor_fd);
  int rightsensor_stat = connect_socket(20016, rightsensor_fd);
  
  return 0;
}

