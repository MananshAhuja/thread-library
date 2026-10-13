#ifndef THREAD_H
#define THREAD_H

#include "structure.h"

extern thread_control_block *current_thread;
extern thread_control_block *thread_list;

void thread_start(thread_control_block *tcb);

void thread_exit(void);

void thread_yield();

void thread_join(thread_control_block *thread);

#endif