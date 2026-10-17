
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

   
}

void scheduler_return(void)
{
    if (current_thread == NULL || scheduler_running == 0)
        return;

    context_switch(&current_thread->context, &scheduler_context);
}

void scheduler_dispatch(void)
{
    thread_control_block *next_thread = scheduler_next();

    if (next_thread == NULL)
        return;

    current_thread = next_thread;
    next_thread->state = RUNNING;

   
    scheduler_running = 1;               //mark the scheduler as active before switching.

    context_switch(&scheduler_context, &next_thread->context);


    scheduler_running = 0;                  //execution resumes here when a thread switches back.
    if (current_thread!=NULL&&current_thread->state==TERMINATED){
        current_thread=NULL;  
      }
}
