#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uintptr_t rsp;
    uintptr_t rbp;
    uintptr_t rbx;
    uintptr_t r12;
    uintptr_t r13;
    uintptr_t r14;
    uintptr_t r15;
    uintptr_t rdi;
} context_t;

/* Register layout follows the reference's x86-64 context. */
void context_init(context_t *context, void *stack, size_t stack_size, void *tcb);
void context_switch(context_t *old_context, context_t *new_context);

#ifdef __cplusplus
}
#endif

#endif /* CONTEXT_H */
