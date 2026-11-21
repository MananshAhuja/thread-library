#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"
#include "context.h"
#include "scheduler.h"
int benchmark_switch_count = 0;
void thread_exit(void)
{
    thread_control_block *old_thread;
    thread_control_block *next_thread;
    if (current_thread == NULL){
        return;
    }
    old_thread = current_thread;
    old_thread->state = TERMINATED;
    if (old_thread->waiting_thread != NULL){ // it check that we have wating thread or not
    
        old_thread->waiting_thread->state = READY; //
        old_thread->waiting_thread = NULL;
    }

    next_thread = scheduler_next();

    if (next_thread != NULL){
         current_thread = next_thread;
        current_thread->state = RUNNING;
        benchmark_switch_count++;
        context_switch(&old_thread->context, &next_thread->context);
        return;
    }
scheduler_return();
}

void thread_start(thread_control_block *tcb)
{
    if (tcb ==NULL || tcb->function ==NULL){
        thread_exit();
        return;
    }
    tcb->function(tcb->arg);
    thread_exit();
}

void thread_yield(void) // it makes the current thread ready and running again
{

    thread_control_block *old_thread;
    thread_control_block *next_thread;
    if (current_thread==NULL){
        return;
    }

    old_thread = current_thread;
    old_thread->state = READY;
    next_thread = scheduler_next();

    if (next_thread == NULL|| next_thread ==old_thread){
        old_thread->state = RUNNING;
        return;
    }
    current_thread = next_thread;
    current_thread->state = RUNNING;
    benchmark_switch_count++;

    context_switch(&old_thread->context, &next_thread->context);}

int thread_join(thread_control_block *thread)          //make the calling thread wait until a particuler target thread finishes
{
    thread_control_block *old_thread;
    thread_control_block *next_thread;

    if (thread==NULL ||current_thread==NULL )
    {
        return -1;
    }
    if (thread==current_thread){
        return -1;
    }
    if (thread->state==TERMINATED){
        return 0;
    }
    if (thread->waiting_thread!=NULL){
        return -1;
    }
    old_thread = current_thread;
    thread->waiting_thread=old_thread;
    old_thread->state = BLOCKED;
    next_thread = scheduler_next();

    if (next_thread == NULL){
    thread->waiting_thread = NULL;
    old_thread->state = RUNNING;
    current_thread=old_thread;
    return -1;
}

    current_thread = next_thread;
    current_thread->state = RUNNING;
    benchmark_switch_count++;

    context_switch(&old_thread->context, &next_thread->context);
    if (thread->state == TERMINATED){
        return 0;
    }
    else{
        return -1;
    }
}

int thread_block(void){
    thread_control_block *old_thread=current_thread;
    if (old_thread==NULL){
        return -1;
    }
    old_thread->state=  BLOCKED;
    thread_control_block *next_thread=scheduler_next();
    if (next_thread ==NULL){
           scheduler_return();      //It prevents the blocked thread from pretending that it can continue
        return -1;
    }
    current_thread=next_thread;
    next_thread->state=RUNNING;
    benchmark_switch_count++;

    context_switch(&old_thread->context,&next_thread->context);
    return 0;
}
