#ifndef __CONTROL_H__
#define __CONTROL_H__

int init_tasks();

/* Sensores*/
void read_speed();
void read_sensores_altura();
void read_sensor_right();
void read_sensor_left();
void return_hortizontal_speed();
void calculate_gravity();

int read_pipe_active_float(int fd, float *value);
int read_socket_passive_float(int fd, float *value);

/* Actuadores */
void actuator_pump1();
void actuator_pump2();
void actuator_drift();
int write_actuator(int fd, char *message);


/*Control*/
void landing_control();
void void_optimal_zone();
void avoid_obstacles();
void go2horitzontal_positon();
void go2vertical_positon();

#endif