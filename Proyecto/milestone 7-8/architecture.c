#include "os.h"

#include "architecture.h"

#include "simulation_parameters.h"
#include <stdlib.h>

// Put all the architecture related initialization code here
PREADYQUEUE tasks_queue=NULL;
PREADYQUEUE tasks_queue2=NULL;
PREADYQUEUE tasks_queue3=NULL;

PSCHEDULER tasks=NULL;
PSCHEDULER tasks2=NULL;
PSCHEDULER tasks3=NULL;

int init_architecture()
{
    Readyqueue_create(&tasks_queue);
    Readyqueue_create(&tasks_queue2);
    Readyqueue_create(&tasks_queue3);
    Scheduler_create(&tasks, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue, 1);
    Scheduler_create(&tasks, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue2, 1);
    Scheduler_create(&tasks, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue3, 1);

    return(0);
}
