#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"
#include "context.h"
thread_control_block *current_thread = NULL;
void thread_exit(void)
{
    thread_control_block *old_thread;
    thread_control_block *next_thread;
    old_thread = current_thread;
    old_thread->state = TERMINATED;
    next_thread = scheduler();
    if (old_thread->waiting_thread != NULL)      //it check that we have wating thread or not
    {
        old_thread->waiting_thread->state = READY;  //
        old_thread->waiting_thread = NULL;
    }

    next_thread = scheduler_next();

    if (next_thread == NULL)
    {
        current_thread = NULL;
        return;
    }
    current_thread = next_thread;
    current_thread->state = RUNNING;

    context_switch(&old_thread->context, &next_thread->context);
}

void thread_start(thread_control_block *tcb)
{
    tcb->function(tcb->arg);
    thread_exit();
}

void thread_yield(void)
{
    thread_control_block *old_thread;
    thread_control_block *next_thread;

    old_thread = current_thread;
    old_thread->state = READY;
    next_thread = scheduler();

    if (next_thread == NULL)
    {
        old_thread->state = RUNNING;
        return;
    }
    current_thread = next_thread;
    current_thread->state = RUNNING;
    context_switch(&old_thread->context, &next_thread->context);
}

void thread_join(thread_control_block *thread)
{
    thread_control_block *old_thread;
    thread_control_block *next_thread;

    if (thread->state == TERMINATED)
    {
        return;
    }
    old_thread = current_thread;
    old_thread->state = BLOCKED;
    thread->waiting_thread = old_thread;
    next_thread = scheduler_next();
    if (next_thread == NULL)
    {
        old_thread->state = RUNNING;
        return;
    }
    current_thread = next_thread;
    current_thread->state = RUNNING;

    context_switch(&old_thread->context, &next_thread->context);
}