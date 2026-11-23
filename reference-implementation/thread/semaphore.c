#include "semaphore.h"
#include "thread.h"
#include <limits.h>

int thread_sem_init(thread_sem_t *sem, unsigned int value)
{
    if (!sem) return -1;
    sem->value = value;
    sem->waiting_queue = NULL;
    sem->initialized = 1;
    return 0;
}

int thread_sem_wait(thread_sem_t *sem){
    thread_control_block **tail;
    if (!sem || !sem->initialized || !current_thread)
        return -1;
    if (sem->value > 0) {
        --sem->value;
        return 0;
    }
    current_thread->mutex_next = NULL;
    tail = &sem->waiting_queue;
    while (*tail) tail = &(*tail)->mutex_next;
    *tail = current_thread;    
  
    if (thread_block()==-1){
        return -1;
    }                 //A post transfers a permit directly to this waiter
    return 0;
 }

int thread_sem_post(thread_sem_t *sem)
{
    thread_control_block *waiter;
     if (!sem || !sem->initialized || !current_thread)
        return -1;
    waiter = sem->waiting_queue;
    if (waiter) {
        sem->waiting_queue = waiter->mutex_next;
        waiter->mutex_next = NULL;
        waiter->state = READY;
    } else {
        if (sem->value == UINT_MAX) return -1;
        ++sem->value;
    }
    return 0;
}

int thread_sem_destroy(thread_sem_t *sem)
{
    if (!sem || !sem->initialized || sem->waiting_queue) return -1;
    sem->initialized = 0;
    return 0;
}
