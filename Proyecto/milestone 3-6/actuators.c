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
#include "sensors.h"
int pump1_fd, pump2_fd, drift_fd;
int init_actuators()
{
  connect_socket(20000, &pump1_fd);
  connect_socket(20001, &pump2_fd);
  connect_socket(20002, &drift_fd);

  return 0;
}
