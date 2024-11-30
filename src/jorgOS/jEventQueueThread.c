#include "jEventQueueThread.h"
#include "jorgOS.h"

/// @brief Post an event to the EventQueue for a specific EQT Threads
/// @param eqt_thread the thread to post the event to
/// @param e the event
void OS_EQT_post(OS_EventQueue_Thread * eqt_thread, J_Event e){
    // The scheduler needs to be called in a critcal section, also you don't
    // want multiple threads having race conditions putting events in queues, or
    // reading the queue.
    J_CRIT_SEC_START();
    J_EventQueue_put(&eqt_thread->event_queue, e);
    // Before the scheduler is called, ready the thready.
    OS_schedule();
    OS_set_EQT_to_ready(eqt_thread);
    J_CRIT_SEC_END();
}