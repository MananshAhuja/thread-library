#include "Semaphore.hpp"

#include <stdexcept>

Semaphore::Semaphore(unsigned int initialValue) {
    if (thread_sem_init(&semaphore_, initialValue) != 0) {
        throw std::runtime_error("Could not initialize thread semaphore");
    }
}

Semaphore::~Semaphore() {
    if (semaphore_.initialized) {
        (void)thread_sem_destroy(&semaphore_);
    }
}

int Semaphore::wait() {
    return thread_sem_wait(&semaphore_);
}

int Semaphore::post() {
    return thread_sem_post(&semaphore_);
}
