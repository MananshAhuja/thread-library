#ifndef THREAD_HPP
#define THREAD_HPP

extern "C" {
    #include "thread.h"
    #include "structure.h"
}

class Thread
{
private:
    thread_control_block tcb;
    void (*function)(void *);
    void *arg;
    bool created;

public:
    Thread(void (*func)(void *), void *argument = nullptr);

    int start();
    int join();

    static void yield();

    ~Thread();
};

#endif
