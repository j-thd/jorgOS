#ifndef __J_EVENTQUEUE_THREAD_H__
#define __J_EVENTQUEUE_THREAD_H__

#include "jTCB.h"
#include "jEvent.h"

// Instead of a thread an EventHandler taking J_Event pointers is necessary to
// run the thread
typedef void (*OS_EventHandler)(J_Event);

// Inherit the regular Thread Control Block, so the OS/scheduler/etc can handle
// it the same way. The scheduler calls OS_EventQueue_pump which deals with all
// the differences.
typedef struct OS_EventQueue_Thread{
    OS_Thread super;
    OS_EventHandler event_handler; 
    J_EventQueue event_queue;
} OS_EventQueue_Thread;

void OS_EventQueue_Thread_start(
    OS_EventQueue_Thread * me, // Pointer to EventQueue Specific TCB.
    uint8_t priority, // Priority
    // Instead of a thread handler an event handler. The OS_thread superclass
    // will instead point to an internal thread handler.
    OS_EventHandler eventHandler, 
    uint32_t * stack,
    uint32_t stackSize,
    J_Event * event_buffer,
    uint8_t event_buffer_size);

// A shared function between EventQueue_Threads that does nothing but dispatch
// events. Altough it feels dangerous, it should be safe to share this function
// amongst multiple threads??? (QUESTION_MARK?)
// Also this OS_EventQueue_pump must go in jorgOS.c so it can access the current thread.

void OS_EventQueue_pump(void);

// Asserts/integrity checks on EventQueue thread
bool OS_assert_not_EventQueue_thread(OS_Thread *);

//  Posting events to the EQT (EventQueueThread)
void OS_EQT_post(OS_EventQueue_Thread * , J_Event);


#endif // __J_EVENTQUEUE_THREAD_H__