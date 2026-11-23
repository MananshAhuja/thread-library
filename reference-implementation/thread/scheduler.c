
#include <stdio.h>
#include<stdlib.h>
#include "scheduler.h"
#include "thread.h"
#include "context.h"

static context_t scheduler_context;
static int scheduler_running = 0;               //to store the state of thread

thread_control_block *scheduler_next(void)
{
    thread_control_block *next_thread;

   
    if (current_thread == NULL)               // if there is no current thread, find the first READY thread.
    {
        next_thread = thread_list;

        while (next_thread != NULL)
        {
            if (next_thread->state == READY)
                return next_thread;

            next_thread = next_thread->next;
        }

        return NULL;
    }

  
    next_thread = current_thread->next;               // search after the current thread first.

    while (next_thread != NULL)
    {
        if (next_thread->state == READY)
            return next_thread;

        next_thread = next_thread->next;
    }


    next_thread = thread_list;                  //wrap around to the beginning of the list.

    while (next_thread != NULL && next_thread != current_thread)
    {
        if (next_thread->state == READY)
            return next_thread;

        next_thread = next_thread->next;
    }

return NULL;   
}

void scheduler_return(void)
{
    if (current_thread == NULL || scheduler_running == 0)
        return;
    benchmark_switch_count++;
    context_switch(&current_thread->context, &scheduler_context);
}

static int scheduler_has_blocked_threads(void)              //It checks whether any thread remains blocked after the scheduler has no READY thread
{
    thread_control_block *thread = thread_list;

    while (thread != NULL)
    {
        if (thread->state == BLOCKED)
        {
            return 1;
        }
        thread = thread->next;
    }

    return 0;
}

int scheduler_dispatch(void)
{
    thread_control_block *next_thread;
    int status=0;

    scheduler_running = 1;

    while ((next_thread = scheduler_next()) != NULL){
        current_thread = next_thread;
        next_thread->state = RUNNING;
        benchmark_switch_count++;
        context_switch(&scheduler_context, &next_thread->context);

        if (current_thread != NULL && current_thread->state == TERMINATED){
            free(current_thread->stack);
            current_thread->stack = NULL;
            current_thread->stack_size = 0;
            current_thread = NULL;
           
        }
    }
    if (scheduler_has_blocked_threads()){
        printf("Scheduler warning:--> deadlock or no runnable threads detected\n");
        status=-1;
    }
    current_thread=NULL;
    scheduler_running = 0;
    return status;
}

int scheduler_run(void)
{
    return scheduler_dispatch();
}