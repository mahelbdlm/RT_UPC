#include "os.h"

#include "architecture.h"

#include "simulation_parameters.h"

// Put all the architecture related initialization code here
int init_architecture()
{
    PREADYQUEUE tasks_queue = NULL; /*To create queues*/
    PSCHEDULER sensors_scheduler = NULL; /*To create schedules*/
    /*Initialize the scheduler. Its job will be to manage the tasks in queue and execute them according to the policy*/
    Scheduler_create(&sensors_scheduler, SCHEDULING_SHORTEST_JOB_FIRST, tasks_queue, 1); 
    Readyqueue_create(&tasks_queue); /*Create a task and enqueue it*/
    return 0;
}
