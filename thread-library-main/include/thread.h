#ifndef THREAD_H
#define THREAD_H

#include "structure.h"

//global thread pointers
extern thread_control_block *current_thread;
extern thread_control_block *thread_list;

//thread management functions

int thread_create(thread_control_block*thread,void(*function)(void *),void *arg);

void thread_start(thread_control_block *tcb);

void thread_exit(void);

void thread_yield(void);

int thread_join(thread_control_block *thread);

void thread_block(void);

#endif