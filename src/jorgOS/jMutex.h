#ifndef __JMUTEX_H__
#define __JMUTEX_H__

#include <stdint.h>
#include <stdbool.h>

#include "jorgOS.h"

// Forward decleration needed here to break cyclical dependence of of jorgOS.h
// and jMutex.H (OS_Thread and J_mutex)

typedef struct OS_Thread OS_Thread;

typedef struct J_mutex{
    // A mutex is owned by a thread and can be the only one to unlock it. Value
    // can also be used to warn if a lock is acquired twice by the same thread. 
    OS_Thread * volatile owned_by; 
    // That's all there is I think. If the pointer is null the mutex is free. If
    // not, then it is locked. Easy-peasy?
} J_mutex;

void J_Mutex_init(J_mutex *);
void J_Mutex_acquire(J_mutex *);
void J_Mutex_release(J_mutex *);
bool J_Mutex_check_for_ready(J_mutex *);

#endif //__JMUTEX_H__