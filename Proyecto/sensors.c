#include "sensors.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int init_sensors()
{

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