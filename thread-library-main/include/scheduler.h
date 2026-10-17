#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "structure.h"
thread_control_block *scheduler_next(void);
void scheduler_run(void);
void scheduler_return(void);
void scheduler_dispatch(void);
#endif