#ifndef MUTEX_H
#define MUTEX_H
#include "structure.h"

typedef struct {
    int locked;        //record the mutex is locked (1)or unlocked (0)
    int owner_thread_id;  //recoords which thread owns the lock
    thread_control_block *waiting_queue;    //it  will track the thread waiting to acquire the mutex
    int initialized;
}thread_mutex_t;

int thread_mutex_init(thread_mutex_t *mutex);     //initializes the mutex

int thread_mutex_lock(thread_mutex_t *mutex);     //request the lock

int thread_mutex_unlock(thread_mutex_t *mutex);   //releases the lock

int thread_mutex_destroy(thread_mutex_t *mutex);  //cleans up the mutex it ios no longer needed

#endif