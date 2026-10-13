#ifndef THREAD_H
#define THREAD_H

#include "structure.h"

#ifdef __cplusplus
extern "C" {
#endif

int thread_create(thread_control_block *thread, void (*function)(void *), void *arg);
void thread_join(thread_control_block *thread);
void thread_exit(void);
void thread_yield(void);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_H */
