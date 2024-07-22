#include "jMutex.h"
#include "jAssert.h"

/// @brief Initiliaze the mutex before use.
/// @param mutex the J_mutex to initialize.
void J_Mutex_init(J_mutex * mutex){
    J_REQUIRE(mutex != (void*)0);
    mutex->owned_by = (void *) 0;
}
/// @brief Attempts to acquire the mutex and locks the thread if not possible.
/// The same thread cannot acquire a mutex twice.
/// @param mutex 
void J_Mutex_acquire(J_mutex * mutex){
    J_CRIT_SEC_START();
    J_REQUIRE(mutex != (void*)0);
    OS_mutex_acquire(mutex);
    // Does the scheduler have to be called? Only when it is going to be block,
    // I think.
    // Might be more correct to let OS_mutex_acquire call OS_schedule();
    //OS_schedule();
    J_CRIT_SEC_END();

}
void J_Mutex_release(J_mutex * mutex){
    J_CRIT_SEC_START();
    J_REQUIRE(mutex != (void*)0);
    OS_mutex_release(mutex);
    OS_schedule();

    J_CRIT_SEC_END();
    

}
bool J_Mutex_check_for_ready(J_mutex * mutex){
    J_REQUIRE(mutex != (void*)0);
    return (mutex->owned_by == (void*) 0);

}