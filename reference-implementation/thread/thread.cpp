
#include "thread.hpp"

Thread::Thread(void (*func)(void *), void *argument)
{
    function = func;
    arg = argument;
    created = false;
}

int Thread::start()
{
    if (created || function == nullptr)
    {
        return -1;
    }

    int result = thread_create(&tcb, function, arg);

    if (result == 0)
    {
        created = true;
    }

    return result;
}

int Thread::join()
{
    if (!created)
    {
        return -1;
    }

    return thread_join(&tcb);
}

void Thread::yield()
{
    thread_yield();
}

Thread::~Thread()
{
    // Thread stack cleanup is handled by the C library.
}
