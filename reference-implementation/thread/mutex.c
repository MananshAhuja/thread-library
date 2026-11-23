#include "mutex.h"
#include "thread.h"
#include <stddef.h>

// It manages locks that help threads safely access shared resources.

int thread_mutex_init(thread_mutex_t *mutex)
{
    if (mutex == NULL){
        return -1;
    }
        mutex->locked=0;
        mutex->owner_thread_id=-1;
        mutex->waiting_queue=NULL;
        mutex->initialized=1;
        return 0;
    
}

static void mutex_wait_enqueue(thread_mutex_t *mutex, thread_control_block *thread)
{
    thread->mutex_next = NULL;
    if (mutex->waiting_queue == NULL)
    {
        mutex->waiting_queue = thread;
        return;
    }
    thread_control_block *current = mutex->waiting_queue;
    while (current->mutex_next != NULL)
    {
        current = current->mutex_next;
    }
    current->mutex_next = thread;
}

static thread_control_block *mutex_wait_dequeue(thread_mutex_t *mutex)
{
    if (mutex->waiting_queue == NULL)
    {
        return NULL;
    }
    thread_control_block *thread = mutex->waiting_queue;
    mutex->waiting_queue = thread->mutex_next;
    thread->mutex_next = NULL;
    return thread;
}

int thread_mutex_lock(thread_mutex_t *mutex)
{
    if (mutex == NULL||!mutex->initialized||current_thread == NULL)
    {
        return -1;
    }

    if (mutex->locked != 0 && mutex->owner_thread_id == current_thread->id)
    {
        return -1;
    }
    if (mutex->locked==0){                  //if mutex is free aquire it
        mutex->locked=1;
        mutex->owner_thread_id=current_thread->id;
        return 0;
    }
    thread_control_block *next=current_thread;
    mutex_wait_enqueue(mutex,next);

    while (mutex->owner_thread_id!=next->id){     //wait untill it is not free
        if (thread_block()==-1){
            return -1;
        }
    }
    return 0;           
}

int thread_mutex_unlock(thread_mutex_t *mutex)              //release the mutex
{
    if (mutex == NULL||!mutex->initialized||current_thread == NULL)
    {
        return -1;
    }

    if (mutex->locked == 0 || mutex->owner_thread_id != current_thread->id)
    {
        return -1;
    }

    thread_control_block *waiting_thread = mutex_wait_dequeue(mutex);
    if (waiting_thread!=NULL){                  //checking if the thread is waiting then the thread remain locked
        mutex->locked=1;                              //lock the mutex for next thread
        mutex->owner_thread_id=waiting_thread->id;    //id is copyed
        waiting_thread->state=READY;                  //make the state ready
    }
    else{
    mutex->locked = 0;                              //if the thread is not waiting the the thread is unlocked
    mutex->owner_thread_id = -1;                    //id is equal to -1
    }
    return 0;
}

int thread_mutex_destroy(thread_mutex_t *mutex)
{
    if (mutex == NULL||!mutex->initialized)
    {
        return -1;
    }
    if (mutex->locked != 0||mutex->waiting_queue!=NULL)
    {
        return -1;
    }
    mutex->owner_thread_id = -1;
    mutex->initialized = 0;
    return 0;
}