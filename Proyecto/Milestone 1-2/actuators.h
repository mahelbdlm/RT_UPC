#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

int init_actuators();
int read_pipe_active_float(int fd, float *value);
static int connect_socket(int port, int *fd);
#endif
