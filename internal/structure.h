#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stddef.h>
#include "context.h"

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} thread_state_t;

typedef struct thread_control_block {
    int id;
    thread_state_t state;
    void (*function)(void *);
    void *arg;
    void *stack;
    struct thread_control_block *next;
    size_t stack_size;
    context_t context;
    struct thread_control_block *waiting_thread;
} thread_control_block;

#endif /* STRUCTURE_H */
