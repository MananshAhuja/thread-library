#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "structure.h"
thread_control_block *scheduler_next(void);
int scheduler_run(void);
void scheduler_return(void);
int scheduler_dispatch(void);
#endif