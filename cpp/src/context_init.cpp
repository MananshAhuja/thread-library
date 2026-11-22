#include <cstddef>
#include <cstdint>

extern "C" {
#include "context.h"
#include "thread.h"
}

extern "C" void context_init(context_t* context, void* stack,
                              std::size_t stackSize, void* threadControlBlock) {
    std::uintptr_t stackTop = reinterpret_cast<std::uintptr_t>(stack) + stackSize;
    stackTop &= ~static_cast<std::uintptr_t>(0x0f);

    // ret enters thread_start with RSP at 8 modulo 16, as required by SysV.
    stackTop -= 2 * sizeof(std::uintptr_t);
    auto* initialStack = reinterpret_cast<std::uintptr_t*>(stackTop);
    initialStack[0] = reinterpret_cast<std::uintptr_t>(&thread_start);
    initialStack[1] = 0;

    context->rsp = stackTop;
    context->rbp = 0;
    context->rbx = 0;
    context->r12 = 0;
    context->r13 = 0;
    context->r14 = 0;
    context->r15 = 0;
    context->rdi = reinterpret_cast<std::uintptr_t>(threadControlBlock);
}
