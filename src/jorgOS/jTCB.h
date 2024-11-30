#ifndef __J_TCB_H__
#define __J_TCB_H__
// A seperate file is needed to define the Thread Control Block (TCB) OS_Thread
// as this will resolve cyclical dependencies, causing issues with unknown
// struct sizes.
//
// C still requires forward declarations in jMutex.h but at least OS_thread can
// now be contained in another struct and be subclassed. So, it does entirely
// resolve cyclical dependencies but moves them here.

#include <stdint.h>

#include "jMutex.h"
#include "jSema.h"

typedef struct J_mutex J_mutex;

// Thread Control Block (TCB)
typedef struct OS_Thread {
    void *sp; // Stack pointer of the thread
    uint32_t timeout; // Down-counter for the time-out delay during which the thread blocks
    uint8_t priority; // Holds the priority of threads.
    J_sema *blocking_sema; // Pointer to semaphore blocking the thread. Null pointer if not blocked by sema.
    J_mutex *blocking_mutex; // Pointer to mutex blocking the thread. Null pointer if not blocked.
} OS_Thread;

#endif // __J_TCB_H__