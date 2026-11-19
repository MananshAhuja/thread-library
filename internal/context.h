#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>
#include <stddef.h>
typedef struct
{
    uintptr_t rsp;
    uintptr_t rbp;
    uintptr_t rbx;
    uintptr_t r12;
    uintptr_t r13;
    uintptr_t r14;
    uintptr_t r15;
    uintptr_t rdi;
} context_t;

void context_init(context_t *context, void *stack, size_t stack_size, void *tcb);

void context_switch(context_t *old_context, context_t *new_context);
#endif
