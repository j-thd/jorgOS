#ifndef __JORGOS_H__
#define __JORGOS_H__

#include <stdint.h>
#include <stdbool.h>

#include "jCritSec.h"
#include "jSerialMonitor.h"
#include "jSema.h"
#include "jMutex.h"


// 32 normal threads plus the Idle Thread
#define MAX_NUMBER_OF_THREADS 33
// Macro to quickly ascertain index of most high-priority-thread ready to run
#define THREAD_WITH_HIGHEST_PRIORITY(READY_SET) (32 - __CLZ(READY_SET))
// Get the priority it based on the priority in the TCB
#define GET_PRIO_BIT(thread_) (thread_->priority - 1U)

// Forward declaration to resolve circular dependency with jMutex.h
typedef struct J_mutex J_mutex;

// Thread Control Block (TCB)
typedef struct OS_Thread {
    void *sp; // Stack pointer of the thread
    uint32_t timeout; // Down-counter for the time-out delay during which the thread blocks
    uint8_t priority; // Holds the priority of threads.
    J_sema *blocking_sema; // Pointer to semaphore blocking the thread. Null pointer if not blocked by sema.
    J_mutex *blocking_mutex; // Pointer to mutex blocking the thread. Null pointer if not blocked.
} OS_Thread;

typedef void (*OS_ThreadHandler)();

void OS_init(); // Initialize the operating system

void OS_run(void); // When the OS is ready, this gives all control to the OS.

void OS_tick(void); // Downcount thread timeout timers and unblock when necessary

// Scheduler functions
void OS_schedule(void); // Handle the scheduling of the threads
void OS_delay(uint32_t); // Delay and block a thread.
bool OS_is_blocked(OS_Thread * volatile);
bool OS_check_for_ready(OS_Thread * volatile);


// Semaphore functions
void OS_sema_block(J_sema *); // Block a thread with a semaphore

// Mutex functions
void OS_mutex_acquire(J_mutex *);
void OS_mutex_release(J_mutex *);

// Callbacks
void OS_onStartup(void);    // Callback function for when the OS-starts.
void OS_onIdle(void);       // Callback function for the idle thread.

// Assertions/integrity checks
bool OS_assert_TCB_integrity(void); // Check if the Thread Control Block is correct at the start of a loop.


void OS_Thread_start(
    OS_Thread *me, // Pointer to a struct containing the Thread Control Block
    uint8_t priority, 
    OS_ThreadHandler threadHandler,
    uint32_t *stack, uint32_t stackSize); // Pointer to top of stack, and the stack size.

#endif // __JORGOS_H__