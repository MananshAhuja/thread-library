#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"
#include "context.h"
thread_control_block *current_thread = NULL;
thread_control_block *thread_list = NULL;
#define DEFAULT_STACK_SIZE (64 * 1024)
int thread_create(thread_control_block *thread, void (*function)(void *), void *arg)
{
    static int id = 0;
    thread->id = id++;
    thread->state = READY;
    thread->function = function;
    thread->arg = arg;
    thread->stack_size = DEFAULT_STACK_SIZE;
    thread->stack = malloc(thread->stack_size);
    if (thread->stack == NULL)
    {
        return -1;
    }
    context_init(&thread->context, thread->stack, thread->stack_size, thread);
    thread->next = thread_list;
    thread_list = thread;
    return 0;
}