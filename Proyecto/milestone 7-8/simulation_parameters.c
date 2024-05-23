#include "simulation_parameters.h"

unsigned long long interval=1000; // Interrupt interval in microseconds

int deterministic = 0;            // The simulation will be deterministic

int disable_crash_detection = 0;  // If 1, disable crash detection

float pump_acceleration = -20.0f;  // Acceleration of a pump
int pump_actuator_delay = 50;     // Delay of the pump actuator in miliseconds

float drift_acceleration = 0.5f;  // Drift acceleratin in any direction
int drift_actuator_delay = 500;   // Delay of the drift actuator in miliseconds

float maximum_vertical_speed = 4.0f; // Maximum vertical speed for crash detection

// Test scenario
//float gravity = 9.8f;               // Gravity in m/s2
//int maximum_terrain_altitude = 7;   // Maximum terrain altitude in m
//int terrain_color = 6;

// Moon scenario
//float gravity = 1.6f;               // Gravity in m/s2
//int maximum_terrain_altitude = 4;   // Maximum terrain altitude in m
//int terrain_color = 7;

// Mars scenario

//float gravity = 3.7f;               // Gravity in m/s2
//int maximum_terrain_altitude = 7;   // Maximum terrain altitude in m
//int terrain_color = 1;

// Earth scenario
float gravity = 9.8f;               // Gravity in m/s2
int maximum_terrain_altitude = 15;  // Maximum terrain altitude in m
int terrain_color = 2;