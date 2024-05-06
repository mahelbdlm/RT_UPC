#ifndef __SENSORS_H__
#define __SENSORS_H__

int init_sensors();
static int connect_pipe(char *pipe_name, int *fd_read, int *fd_write);

#endif
