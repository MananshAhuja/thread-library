#pragma once

extern "C" {
#include "mutex.h"
}

class Mutex {
public:
    Mutex();
    ~Mutex();

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;
    Mutex& operator=(Mutex&&) = delete;

    int lock();
    int unlock();

private:
    thread_mutex_t mutex_{};
};

class LockGuard {
public:
    explicit LockGuard(Mutex& mutex);
    ~LockGuard();

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

private:
    Mutex& mutex_;
};
