#include "context.h"
#include "thread.h"

void context_init(context_t *context, void *stack, size_t stack_size, void *tcb)
{
    uintptr_t stack_top = (uintptr_t)stack + stack_size;

    stack_top &= ~(uintptr_t)0xf;
stack_top -= 2 * sizeof(uintptr_t);
*(uintptr_t *)stack_top = (uintptr_t)thread_start;

    context->rsp = stack_top;
    context->rbp = 0;
    context->rbx = 0;
    context->r12 = 0;
    context->r13 = 0;
    context->r14 = 0;
    context->r15 = 0;
    context->rdi = (uintptr_t)tcb;
}
   
