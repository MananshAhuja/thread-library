#pragma once

extern "C" {
#include "semaphore.h"
}

class Semaphore {
public:
    explicit Semaphore(unsigned int initialValue);
    ~Semaphore();

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore(Semaphore&&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;

    int wait();
    int post();

private:
    thread_sem_t semaphore_{};
};
