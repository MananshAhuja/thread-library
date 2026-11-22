#include <iostream>
#include "thread.hpp"

#include <iostream>
#include "thread.hpp"

extern "C" {
    #include "scheduler.h"
}

void worker_A(void *arg)
{
    for (int i = 1; i <= 3; i++)
    {
        std::cout << "Thread A: iteration " << i << std::endl;
        Thread::yield();
    }

    std::cout << "Thread A: waiting for Thread B..." << std::endl;

    // Wait for Thread B to finish.
    extern Thread *threadBObject;
    int result = threadBObject->join();

    if (result == 0)
    {
        std::cout << "Thread A: Thread B completed!" << std::endl;
    }
    else
    {
        std::cout << "Thread A: Join failed!" << std::endl;
    }

    std::cout << "Thread A: continuing after join." << std::endl;
}

void worker_B(void *arg)
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "Thread B: iteration " << i << std::endl;
        Thread::yield();
    }

    std::cout << "Thread B: work completed." << std::endl;
}

void worker_C(void *arg)
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "Thread C: iteration " << i << std::endl;
        Thread::yield();
    }
}

Thread *threadBObject = nullptr;

int main()
{
    Thread A(worker_A);
    Thread B(worker_B);
    Thread C(worker_C);

    threadBObject = &B;

    std::cout << "Starting Thread Join Demo" << std::endl;

    A.start();
    B.start();
    C.start();

    scheduler_dispatch();

    std::cout << "All threads completed." << std::endl;

    return 0;
}
