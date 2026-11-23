#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "thread.h"
#include "context.h"
thread_control_block *thread_list = NULL;
thread_control_block *current_thread = NULL;
#define DEFAULT_STACK_SIZE (64 * 1024)
int thread_create(thread_control_block *thread, void (*function)(void *), void *arg){
    static int id = 0;
    if (thread == NULL || function == NULL){
    return -1;
}
void *stack = malloc(DEFAULT_STACK_SIZE);

    if (stack == NULL){
        return -1;
    }
    
    thread->id = id;
    thread->state = READY;
    thread->function = function;
    thread->arg = arg;
    thread->waiting_thread = NULL;
    thread->mutex_next = NULL;
    thread->stack_size = DEFAULT_STACK_SIZE;
    thread->stack = stack;

    context_init(&thread->context, thread->stack, thread->stack_size, thread);

    thread->next = thread_list;
    thread_list = thread;

    id++;
    return 0;
}
