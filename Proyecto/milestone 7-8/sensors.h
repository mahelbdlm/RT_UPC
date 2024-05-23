#ifndef __SENSORS_H__
#define __SENSORS_H__

int init_sensors();
int connect_pipe(char *pipe_name, int *fd_read, int *fd_write);
int connect_socket(int port, int *fd);
extern int speed_fd_read, speed_fd_write, alt1_fd, alt2_fd, alt3_fd, leftsensor_fd, rightsensor_fd;
#endif
