#ifndef THREAD_INTERNAL_H
#define THREAD_INTERNAL_H

#include "thread.h"

#ifdef __cplusplus
extern "C" {
#endif

extern thread_control_block *current_thread;
extern thread_control_block *thread_list;
void thread_start(thread_control_block *tcb);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_INTERNAL_H */
