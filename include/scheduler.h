#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "structure.h"

#ifdef __cplusplus
extern "C" {
#endif

thread_control_block *scheduler_next(void);

#ifdef __cplusplus
}
#endif

#endif /* SCHEDULER_H */
