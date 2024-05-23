#include "os.h"

#include "architecture.h"

#include "simulation_parameters.h"
#include <stdlib.h>

// Put all the architecture related initialization code here
PREADYQUEUE tasks_queue=NULL;

PSCHEDULER tasks=NULL;
PSCHEDULER tasks2=NULL;
PSCHEDULER tasks3=NULL;

int init_architecture()
{
    Readyqueue_create(&tasks_queue);
    Scheduler_create(&tasks, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue, 1);
    //Scheduler_create(&tasks2, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue, 1);
    //Scheduler_create(&tasks3, SCHEDULING_EARLIEST_DEADLINE_FIRST, tasks_queue, 1);

    return(0);
}
