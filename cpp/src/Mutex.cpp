#include "Mutex.hpp"

#include <stdexcept>

Mutex::Mutex() {
    if (thread_mutex_init(&mutex_) != 0) {
        throw std::runtime_error("Could not initialize thread mutex");
    }
}

Mutex::~Mutex() {
    if (mutex_.initialized) {
        (void)thread_mutex_destroy(&mutex_);
    }
}

int Mutex::lock() {
    return thread_mutex_lock(&mutex_);
}

int Mutex::unlock() {
    return thread_mutex_unlock(&mutex_);
}

LockGuard::LockGuard(Mutex& mutex) : mutex_(mutex) {
    if (mutex_.lock() != 0) {
        throw std::runtime_error("Could not lock thread mutex");
    }
}

LockGuard::~LockGuard() {
    (void)mutex_.unlock();
}
