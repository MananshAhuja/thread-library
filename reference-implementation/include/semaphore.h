#ifndef THREAD_SEMAPHORE_H
#define THREAD_SEMAPHORE_H
#include "structure.h"

typedef struct {
    unsigned int value;
    int initialized;
    thread_control_block *waiting_queue;
} thread_sem_t;

int thread_sem_init(thread_sem_t *sem, unsigned int value);
int thread_sem_wait(thread_sem_t *sem);
int thread_sem_post(thread_sem_t *sem);
int thread_sem_destroy(thread_sem_t *sem);
#endif
