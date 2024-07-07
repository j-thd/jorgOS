#include <stdint.h>
#include <stdbool.h>

#include "jAssert.h"
#include "jorgOS.h"
#include "jCritSec.h"
#include "jSema.h"

/// @brief Initialize and register the semaphore with jorgOS
/// @param sema J_sema object 
/// @param initial_count The count at which the semaphore begins.
/// @param max_count The maximum count the semaphore allows. 1 is a binary semaphore
void J_sema_init(J_sema *sema, uint8_t initial_count, uint8_t max_count){
    J_REQUIRE(initial_count <= max_count);
    sema->count = initial_count;
    sema->max_count = max_count;
}

/// @brief Add 1 to the counter to signal that (the) resource(s) are available.
/// This will results 
/// @param sema J_sema object
void J_sema_signal(J_sema *sema){
    J_REQUIRE(sema != (void *)0);
    J_REQUIRE(sema->count != sema->max_count);
    sema->count++;
}
void J_sema_wait(J_sema *sema){
    J_REQUIRE(sema != (void *)0);
    // Don't want race conditions read/writing the semaphore counts, obviously.
    J_CRIT_SEC_START();
    // If the semaphore has at least 1 resource, it can simply consume the
    // resource and the function can return.
    if (sema->count > 0){
        sema->count--;
        // Nothing special needs te be done if the count reaches zero, because
        // this means no thread should have been waiting on this semaphore, and
        // OS_sema_set in jorgOS.c should be all zeroes already. Nothing needs
        // to be updated, no priorities should change.
    } 
    else { // This means sema->count is 0.
        // One important observation here: the OS cannot be called directly to
        // schedule a new thread, as the critical section will never be exited
        // in that case. The semaphore update code, however, must be called in a
        // critical sections, as there would be conflicts if there is a
        // context-switch to another thread wanting to update the semaphore
        // state. The solution is to do something similar to OS_delay, which
        // calls the scheduler when everything is arranged.
        OS_sema_block(sema);
    }
    J_CRIT_SEC_END();

}

 bool J_sema_check_for_ready(J_sema *sema){
    J_REQUIRE(sema != (void *)0);
    J_REQUIRE(sema->count <= sema->max_count);
    // If the semaphore counter is larger than 1, subtract 1 resource and
    // unblock the thread. The unblocking of the thread is done by the scheduler
    // by setting the blocking_sema pointer in the Thread Control Block to null.
    // This is the reason J_sema_check_for_ready can only be called by
    // OS_schedule, as subtracting a resource only correct if the thread is
    // actually unblocked.
    if (sema->count == 0 ){
        return false;
    }
    else {
        --sema->count; // Reduce counter and unblock thread in OS_schedule
        return true;
    }


}