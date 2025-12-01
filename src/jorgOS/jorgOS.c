#include <stdint.h>
#include <stdbool.h>

#include "jorgOS.h"
#include "jAssert.h"
#include "jCritSec.h"
#include "jEventQueueThread.h"
#include "jEvent.h"
#include "runtime_environment.h"

// Port should go here instead #include "runtime_environment.h"



// NOTE: volatile comes after the asterix as the POINTER needs to be volatile.
OS_Thread * volatile OS_curr; // Pointer to the current thread
OS_Thread * volatile OS_next; // Pointer to the next thread.


// Idle thread and its stack 
OS_Thread idle_thread;
// Note: it would probably have this defined by the user, but for now it will be
// done here since (idle) stack integrity checks require jorgOS.c top know about the
// stack size and location
#define IDLE_THREAD_STACKSIZE 100U
uint32_t idle_thread_stack[IDLE_THREAD_STACKSIZE];

//All threads will be stored with pointers to the Thread Control Blocks
OS_Thread *OS_thread_list[MAX_NUMBER_OF_THREADS]; 
// A 32-bit mask for ready threads. Idle is always ready, and not in this bitmask
uint32_t OS_ready_set;
// A 32-bit mask holding the set of delayed threads. Threads can be delayed and NOT ready due to mutexes or stuff like that.
uint32_t OS_delayed_set;
// A 32-bit mask holding the set of EventQueue_Threads. This is probably only
// required for some stringent design-by-contract checking. Threads in this set
// should never be blocked, so asserts can trip if mutexes, or delays are used
// in those threads.
uint32_t OS_event_queue_set;

void main_idle_thread(void){
    while(1){
        J_ASSERT_STACK_INTEGRITY(idle_thread_stack, IDLE_THREAD_STACKSIZE);
        OS_onIdle();
    }
}

void OS_init(){
    // Set the Pend SV interupt priority to the lowest level
    SCB->SHP[10] |= (0xF << 4); // IN Core_CM4, SHP is a pointer to uint8_t. So, it is a bit convoluted to access the right register.
    // This ensures run-to-completion of other interupts.

    // Critical section code must be initialized before the OS takes control
    // to deal with nested critical sections.
    J_crit_sec_init();
    // The idle thread is created with priority 0. Higher means more priority.
    OS_Thread_start(&idle_thread,
        0U, // Priority
        main_idle_thread,
        idle_thread_stack,
        sizeof(idle_thread_stack));
}

void OS_delay(uint32_t ticks_delay){
    J_CRIT_SEC_START(); // Race condition galore here. OS_current cannot switch in the meantime, and neither can be the readyset bits be altered.

    // OS_delay must not be called in the idle thread.
    J_REQUIRE(OS_curr != OS_thread_list[0]);
    // OS_delay cannot be balled in an EventQueuethread
    J_ASSERT_NOT_EVENTQUEUE_THREAD(OS_curr);
    // A thread should can only be blocked for ONE reason at a time, so no
    // semaphores should be registered in the Thread Control Block.
    J_REQUIRE(OS_curr->blocking_sema == (void *)0);
    J_REQUIRE(OS_curr->blocking_mutex == (void *)0);
    
    // OS_delay can only be called by the current thread.
    // Add the time-out timer to the current thread.
    OS_curr->timeout = ticks_delay;
    uint8_t priority_bit = OS_curr->priority -1U;
    // Unset the ready bit for the current thread.
    // Remeber the -1 because the 0 thread is the idle thread.
    OS_ready_set &= ~(1U << priority_bit);
    OS_delayed_set |= 1U << priority_bit;
    // Schedule the next thread, because we want to get out of this thread, of course!
    OS_schedule();
    J_CRIT_SEC_END();
}

/// @brief OS_sema_block. Should not be called by user but only by
/// J_sema_wait(). It sets the blocking_sema pointer, which is enough to make
/// the scheduler take into account the semaphore. Must be called in a critical
/// section. 
/// @param sema Pointer to the blocking semaphore (not null).
void OS_sema_block(J_sema *sema){
    J_REQUIRE_IN_CRIT_SEC;
    
    J_REQUIRE(sema != (void *)0);
    // OS_sema_block must not be called in the idle thread.
    J_REQUIRE(OS_curr != OS_thread_list[0]);
    // Must not be called in an eventqueue thread
    J_ASSERT_NOT_EVENTQUEUE_THREAD(OS_curr);
    // A thread should only be blocked for ONE reason at all times, so it should
    // not have a timer or be in the delayed set. Also, it should still be
    // ready-to-run.
    J_REQUIRE(OS_curr->timeout == 0);
    uint8_t priority_bit = OS_curr->priority -1U;
    J_REQUIRE((OS_delayed_set & (1U << priority_bit)) == 0U);
    J_REQUIRE((OS_ready_set & (1U << priority_bit)) != 0U);
    OS_curr->blocking_sema = sema;
    // After the the blocking semaphore is set, we just need to call the
    // scheduler. In its current implementation it will simply check the
    // blocking_sema pointer when deciding to schedule a thread or not.
    OS_schedule();
}

// Mutex functions
void OS_mutex_acquire(J_mutex * mutex){
    J_REQUIRE_IN_CRIT_SEC;
    // Must not be called in an eventqueue thread
    J_ASSERT_NOT_EVENTQUEUE_THREAD(OS_curr);
    J_REQUIRE(mutex != (void*)0);
    //Mutex cannot be acquired again by the same thread.
    J_REQUIRE(mutex->owned_by != OS_curr); 

    // If the mutex is owned by no thread, simply assign the owner
    if(mutex->owned_by == (void*)0){
        mutex->owned_by = OS_curr;
    }
    // Otherwise set the blocking_mutex pointer in the Thread Control Block, and
    // call the scheduler before returning
    else {
        OS_curr->blocking_mutex = mutex;
        OS_schedule();
        // This will block the current running thread.
    }
    return;
}
void OS_mutex_release(J_mutex * mutex){
    J_REQUIRE_IN_CRIT_SEC;
    // Must not be called in an eventqueue thread
    J_ASSERT_NOT_EVENTQUEUE_THREAD(OS_curr);
    J_REQUIRE(mutex != (void*)0);
    // Mutex can only be released by the thread that acquired it, and it can of
    // course not be released when it is owned by no-one.
    J_REQUIRE(mutex->owned_by == OS_curr);
    // All that is required is to unset the owned_by parameter, which signifies
    // to the scheduler that another thread blocked by the mutex can now run.
    // The logic of unlocking threads is contained there.
    mutex->owned_by = (void *)0;
    OS_schedule();

}

// SCHEDULER FUNCTIONS

/// @brief Takes a thread and returns whether it is blocked by a semaphore or a
/// mutex. 
/// @param thread OS_Thread to be checked for block
/// @return bool telling whether thread is blocked or no
bool OS_is_blocked(OS_Thread * volatile thread){
    J_REQUIRE_IN_CRIT_SEC;
    J_REQUIRE(thread != (void*)0);

    // Check that the thread is only blocked for one reason.
    J_REQUIRE_ZERO_OR_ONE_BLOCKING_REASON(thread);
    // Since the asserts ensures only one blocking reason, we can safely do a
    // simple OR || statement.
    return thread->blocking_mutex != (void *)0  || \
        thread->blocking_sema != (void *) 0;
}

/// @brief Function to check if threads are ready AND update the thread control
/// blocks to the new state if they are ready. The scheduler will only use the
/// return value to schedule the thread and performs no updates of the TCB of
/// its own. 
/// @param thread 
/// @return bool - true if the thread is ready, false if not. Side effect is
/// that the Thread Control Block is updated.
bool OS_check_for_ready(OS_Thread * volatile thread){
    J_REQUIRE_IN_CRIT_SEC;
    J_REQUIRE(thread != (void*)0);
    // This check should never run on an eventqueue thread if the scheduler is
    // working properly, as a ready eventqueue thread can never be blocked.
    J_ASSERT_NOT_EVENTQUEUE_THREAD(thread);
    // Check that the thread is only blocked for one reason.
    J_REQUIRE_ONE_BLOCKING_REASON(thread);
    // Since the thread has been asserted to be blocked for one reason, we can
    // simply go through the possible blocking reasons one-by-one in any random
    // order.
    if (thread->blocking_sema != (void *)0 ){
        // The thread is blocked by a semaphore
        if (J_sema_check_for_ready(thread->blocking_sema)){
                // The semaphore counter was above 0, and J_sema_check_for_ready
                // has reduced the counter.

                //The thread is no longer blocked by a semaphore, so the
                //pointer should be set to 0.
                thread->blocking_sema = (void *) 0;
                return true; // thread is ready to run.
            }
            else {
                // thread is not ready to run.
                return false;
            }
    }
    else if (thread->blocking_mutex != (void *)0)
    {
        //Thread is blocked by a mutex.
        if (J_Mutex_check_for_ready(thread->blocking_mutex)){
            // The owning thread has released the mutex, so this checked thread
            // no longer is blocked by this mutex.
            // This waiting thread must become the new owner.
            thread->blocking_mutex->owned_by = thread;
            // To check the global state of the scheduler, the mutex must now be
            // owned OS_next which was the candidate thread for running, but
            // should now be the next thread to run. The scheduler works by
            // simply setting OS_next to a candidate thread to run, and going
            // down the priority list until it needs not to be changed any more.
            // This check must happen here, before the pointer is set to null again.
            J_ENSURE(thread->blocking_mutex->owned_by == OS_next);
            // Finally, the Thread Control Block must be updated to set the
            // blocking mutex to null, which tells the scheduler the thread is
            // released. This needs to happen at the end, so the pointer to the
            // mutex is still available to change the owner.
            thread->blocking_mutex = (void *) 0;
            return true; // thread is ready to run.
        }
        else {
            return false; // thread is not ready to run.
        }
    }

    // One of the if-statements above must have triggered before, because
    // this function should only be called on a blocked thread. Each
    // if-statement should return true or false. Therefore the next line of code
    // should never be executed.
    J_ERROR();
    
    // Just a return to soothe the compiler. J_ERROR never returns. Compiler
    // does not understand.
    return false;
}

//This function should always be called in a critical section.
void OS_tick(void){
    J_REQUIRE_IN_CRIT_SEC;
    // Due to the priority-scheduling, and the fact that not ready threads are not neccessarily delayed
    // We must go through the list of DELAYED threads and reduce their timeout downcounter.
    // We copy the delayed bitset, and unset the bit of the threads one by one.
    uint32_t delayed_set_working_copy = OS_delayed_set;
    while (delayed_set_working_copy != 0U){
        // Establish the highest priority-thread to have the timeout downcounter reduced on.
        OS_Thread *thread = OS_thread_list[THREAD_WITH_HIGHEST_PRIORITY(delayed_set_working_copy)];
        uint8_t priority_bit; // Often re-used value in this snippet of code
        // Check if the thread exists, and the timeout is not 0 already.
        J_ASSERT(thread != (void *)0 && thread->timeout != 0U);
            
        
        // Reduce the timeout and unsit the bit in the working copy
        --thread->timeout;
        
        priority_bit = GET_PRIO_BIT(thread);
        // If the thread timeout hits 0, the thread is no longer delayed and ready to run
        if (thread->timeout == 0U){
            OS_ready_set |= 1U << priority_bit; // Set the thread to ready.
            OS_delayed_set &= ~(1U << priority_bit); // Unset the delayed bitgit 
        }
        // Unset the bit in the working copy.
        delayed_set_working_copy &= ~( 1U << priority_bit );
    }
}


void OS_run(void){
    // Callback to configure and start interupts
    OS_onStartup();
    
    // To start the OS, a thread must be scheduled, and the interupts can be enabled.
    __disable_irq();
    OS_schedule();
    __enable_irq(); 
    
    // This part will never execute! OS_Schedule will set an interupt to switch the context.
    // When the interupts are enabled, this will immediately switch the context to the first thread.
    J_ERROR();
}

// This function must always be called with interupts DISABLED.
//  The interupt disable cannot be placed in this function, as enabling it again in the end could re-enable interupts in a function that needed it disabled.
void OS_schedule(void){
    J_REQUIRE_IN_CRIT_SEC;
    // Priority based-scheduling. Highest priority runs first. 0 is the idle thread.
    // Normal threads range from 1-32.
    // Additionally, a thread that is not delayed and ready to run, could be
    // blocked by a semaphore, so the blocking_sema will be checked too. If it
    // is blocked the next ready thread must run.

    uint32_t OS_ready_set_working_copy = OS_ready_set;

    // Go through the ready(not delayed by timer) until one not blocked by a
    // semaphore is found. 

    // Since this loop should always break when the correct thread is found,
    // there is no sensible condition to check for, unless convoluted isDone
    // bools are added instead of the simpler to read break statements.
    //
    // As a precaution, however, there will be a counter that triggers an
    // assertion if there are more loops than there are threads, which should
    // never happen.
    uint8_t loop_counts = 0;
    while(1)
    {
        // Intentionally crash the code if this loop runs more often than there
        // are threads in existence.
        loop_counts++;
        J_ASSERT(loop_counts <= MAX_NUMBER_OF_THREADS);

        // Find the next thread not delayed by a timer.
        OS_next = OS_thread_list[THREAD_WITH_HIGHEST_PRIORITY(OS_ready_set_working_copy)];
        J_ASSERT(OS_next != (void *)0); // The thread must exist
        // Check if this thread is blocked by a semaphore.
        
        if ( !OS_is_blocked(OS_next) ){
            // OS_next is the next thread to run.
            break;
        }
        else {
            // If the thread is blocked, check it was not an eventqueue thread.
            // That should enver happend
            J_ASSERT_NOT_EVENTQUEUE_THREAD(OS_next);
            // If the thread is blocked, OS_check_for_ready can be called, which
            // will handle it based on the blocking reason (mutex or semaphore)
            if (OS_check_for_ready(OS_next)){
                // OS_check_for_ready does all the updating of the mutexes and
                // semaphores, as well as the Thread Control Blocks. So, only a
                // break statement is needed.

                break; // OS_next is ready to run, so the loop can break.
            }
            else {
                // The thread could not be unblocked yet. Unset the priority bit
                // in the working set of ready-to-run(not delayed by timer)
                // threads. Loop again to find the next ready-to-run thread that
                // is not blocked.
                uint8_t priority_bit = OS_next->priority -1U;
                OS_ready_set_working_copy &= ~(1 << priority_bit);
            }
        }
    }

    // Trigger the PendSV interupt handler when the next thread differs from the current one
    if (OS_next != OS_curr) {
        SCB->ICSR |=  SCB_ICSR_PENDSVSET_Msk; // Set the PendSV bit in the INTCTRL register of the SCB.
    }
}

/// @brief This checks whether the
/// scheduler is working appropriately and not activating threads that should
/// not be activated (delayed, or blocked by semaphores, mutex, etc..). This
/// assert should always return true in a running thread.
/// @param  
/// @return [bool] Returns true if the Thread Control Block is as expected in a
/// running thread.
bool OS_assert_TCB_integrity(void){
    return OS_curr->timeout == 0 && OS_curr->blocking_sema == (void *) 0\
        && OS_curr->blocking_mutex == (void *)0;
}

void OS_Thread_start(
    OS_Thread *me, // Pointer to a struct containing the Thread Control Block
    uint8_t priority, // Thread priority is assigned at start. Higher numbers run first. (limited to 32 threads+ IDLE thread).
    OS_ThreadHandler threadHandler,
    uint32_t *stack, uint32_t stackSize){ // Pointer to top of stack, and the stack size. I think the stack size must be in bytes to get the desired size.
    
/****
        Preparation of the stack happens here.
******/
        
    J_REQUIRE(stackSize > 64U); // Absolute minimum stack size.
    // Initialize the stack pointer to the top of stack, aligned at the 8-byte boundary.
    uint32_t *sp = (uint32_t *)((((uint32_t)stack + stackSize)/8U)*8U); // This is not(?) pointer arethmatic but simply casting the address to unsigned int and adding bytes to it. Finally this address becomes a pointer again.
    uint32_t *stack_limit; // Here the bottom of the stack can be saved.
        
    //Fabricate Cortex-M ISR stack frame for thread stack
                
    // First the registers that will be pushed and popped on the scack by default (AAPCS standard!)
    *(--(sp)) =  (1U << 24); // Thumb bit set in xPSR
    *(--(sp)) = (uint32_t)threadHandler; // Pointer to thread in PC
    *(--(sp)) = 0x0EU; // LR
    *(--(sp)) = 0x0CU; // R12
    *(--(sp)) = 0x03U; // R3
    *(--(sp)) = 0x02U; // R2
    *(--(sp)) = 0x01U; // R1
    *(--(sp)) = 0x00U; // R0
    // Additional registers that need to be stored and recovered upon entering/exiting threads.
    // This does not happen automatically.
    *(--(sp)) = 0x0BU; // R11
    *(--(sp)) = 0x0AU; // R10
    *(--(sp)) = 0x09U; // R9
    *(--(sp)) = 0x08U; // R8
    *(--(sp)) = 0x07U; // R7
    *(--(sp)) = 0x06U; // R6
    *(--(sp)) = 0x05U; // R5
    *(--(sp)) = 0x04U; // R4
        
    me->sp = sp;
    stack_limit = (uint32_t *)(((((uint32_t)stack - 1U) / 8U) + 1U) * 8U);
    for (sp = sp - 1U; sp >= stack_limit; --sp){
        // A default stack filler value is used, so a stack overflow can be
        // detected. This is done with J_ASSERT_STACK_INTEGRITY
        *sp = J_DEFAULT_STACK_FILLER; 

    }

/*******
        Storing the the thread in the thread list, assigning priority,
            and readying the thread happens here.
******/
    

    // There is no need for an assert for the number of threads.
    // There can only be one thread per priority number.
    // The priority is 1-indexed for normal threads, The idle thread has priority 0.
    J_ASSERT(priority < MAX_NUMBER_OF_THREADS);
    J_ASSERT(OS_thread_list[priority] == (void *)0U); // Should be a null pointer if no thread is assigned yet at this priority
    
    OS_thread_list[priority] = me;
    me->priority = priority; // The  TCB must know about its own priority, finding it in a list is a bit idea.
    //Set the thread to be ready to run, but only if it is not the IDLE thread
    if (priority > 0U){
        OS_ready_set |= (1U << (priority-1U) );
    }

    // Initializing the semaphore pointer to 0;
    me->blocking_sema = (void *)0;
    me->blocking_mutex = (void *)0;

}

/**************************************
* J_EVENTQUEUE_THREAD 
*/

// This functionality mostly needs to be here because there's always small core
// function of jorgOS.c that need to be accessed.

/// @brief Start a special-purpose thread that does not but service an event queue at the given
/// priority with respect to other (event queue) threads. 
/// @param me pointer to the EventQueue Thread Control Block
/// @param priority priority
/// @param eventHandler pointer to OS_EventHandler for the Event Queue
/// @param stack pointer to the stack of this thread
/// @param stackSize size of the stack
/// @param event_buffer pointer to the J_Event * buffer
/// @param event_buffer_size size of the J_event buffer (in terms of elements
/// and not bytes)

void OS_EventQueue_Thread_start(
    OS_EventQueue_Thread * me, // Pointer to EventQueue Specific TCB.
    uint8_t priority, // Priority
    // Instead of a thread handler an event handler. The OS_thread superclass
    // will instead point to an internal thread handler.
    OS_EventHandler eventHandler, 
    uint32_t * stack,
    uint32_t stackSize,
    J_Event * event_buffer,
    uint8_t event_buffer_size){

    // OS_Thread_Start should already check the validity of all inputs but
    // event_buffer and event_buffer_size, so let's skip that.
    
    J_ASSERT(event_buffer != (void *) 0); 
    J_ASSERT(event_buffer_size > 1);
    // Start a regular thread with the reguler OS_Thread superclass but using
    // the special-purpose event pump which eventually dispatches events for
    // each thread. 
    OS_Thread_start((OS_Thread *)me, priority, &OS_EventQueue_pump, stack, stackSize);
    // Assign the OS_Eventhandler
    me->event_handler = eventHandler;

    // This snippet here is the reason why this must be in this file. We want to
    // set what type of thread this is. Also remember that OS_thread sets the
    // thread to be ready to run, so it must actually be ready with the initial event.
    OS_event_queue_set |= 1U << (priority-1);

    // Initialize an EventQueue
     J_EventQueue_init(&me->event_queue, event_buffer, event_buffer_size);
    
    // Post the initial event in the queue
    J_Event e_init = {INIT_SIG};
    J_EventQueue_put(&me->event_queue, e_init);
}

void OS_EventQueue_pump(void){
    // Checks for the first time the event_queue runs.
    // The current thread must be an EQ thread
    J_ASSERT_EVENTQUEUE_THREAD(OS_curr);
    // Check if an event handler is assigned
    J_ASSERT( ((OS_EventQueue_Thread*)OS_curr)->event_handler != (void *)0 );
    while(1){
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_EVENTQUEUE_THREAD(OS_curr);
        
        // A critical section starts here, to make sure all event handling will
        // be Run-To-Completion (RTC)
        J_CRIT_SEC_START();

        // The EventQueue_pump should never be running from the top when the
        // event-queue is empty. So, while there could be a simply if-statement
        // to check this, an immediate halt is preferred to prevent silent issues.
        J_ASSERT_EVENTQUEUE_NOT_EMPTY(OS_curr);

        // Three things need to be done. 
        // * Dispatch the event to the OS_EventHandler,
        // * Check if queue is now empty and set the thread to not ready if that
        //   is the case.
        // * Leave the critical section so the scheduler can potentially
        //   intervene and swap priorities.
        
        // Get the pointer to the subclass once for convencience
        OS_EventQueue_Thread * event_thread = (OS_EventQueue_Thread*)OS_curr;


        // Dispatch event by calling event_handler
        event_thread->event_handler(
            J_EventQueue_get(&event_thread->event_queue) );
        

        // Check if queue is empty, and unready the thread if necessary
        if ( J_EventQueue_isEmpty( &event_thread->event_queue ) ) {
            // Unset the ready bit
            OS_ready_set &= ~ (1U << (OS_curr->priority-1U) );
            // If there is nothing to wrong, it is best to call the scheduler.
            OS_schedule();
        }

        // The code was in a critical section to ensure Run-To-Completion.
        J_CRIT_SEC_END();
        // This is where the thread is guaranteed to switch if the queue was
        // empty, because the scheduler was called. This means multiple events
        // can be handled each time the thread is activated. The OS will also
        // intervene here if the thread has been running longer than a tick and
        // another higher priority thread is ready now. This gives an
        // interesting insight in my scheduler and event handling solution. I
        // should not want the event handling to take longer than a tick so the
        // priority-based scheduling keeps working in a timely manner.

    }
}

void OS_set_EQT_to_ready(OS_EventQueue_Thread * eqt_thread){
    J_ASSERT( eqt_thread != (void *)0 );
    // Compiler should check this already, but it doesn't hurt to constantly
    // check if my OS is not screwing up
    J_ASSERT_EVENTQUEUE_THREAD( (OS_Thread *)eqt_thread );
    // I don't think there are race conditions, as unreadying only happens when
    // in a critical section, but can't hurt to be sure. This is called in a
    // critical section anyway.
    J_REQUIRE_IN_CRIT_SEC;
    OS_ready_set |= 1U << GET_PRIO_BIT((OS_Thread *)eqt_thread);
    // That's it. Three asserts and one line of code. :P
}

__attribute__((naked)) void PendSV_Handler(void){
    // ASSEMBLY FOR THE CONTEXT SWITCH
//     __asm("SUB           sp,sp,#0x04"); // Is this needed?
//   752:   __ASM volatile ("cpsid i" : : : "memory"); 
            __asm("     CPSID            I"); //Disable interupts.
//    68:     if (OS_curr != (OS_Thread *)0) { // Check if OS_current is not a null pointer, which it can be at the start of the program 
            // Make the OS_curr and OS_next labels available.
            //__asm("     IMPORT           OS_curr");
            //__asm("     IMPORT           OS_next");
            //Load the ADDRESS of OS_curr pointer in R0. This is not the pointer, but the address of the pointer value.
            __asm("     LDR              r0,=OS_curr");
            // Then the value at the address of the OS_curr pointer must be read.
            __asm("     LDR              r0,[r0]");
            // If this value is zero, we are dealing with a null pointer, so a conditional branch on zero is made.
            // When there is a null pointer, there was no current thread, 
            // and we can immediately restore the context of the first thread we run. 
            // There is no need to push the registers r4-r11 onto the stack, and save the stack pointer.
            // So a label must be provided just beyond the point where it would normally save the current context by pushing it onto the current threads stacks
            __asm("     CBZ              r0,PendSV_restore"); 
    // START OF PART WHERE THE CONTEXT IS BEING SAVED
            //IGNORE? B             0x0000075E // Is this jump really needed? Is it an alignment, thing?
            // Push r4-r11 onto the stack. The AACPS does not demand that they are preserved between function calls
            __asm("     PUSH             {r4-r11}");
            // Register r0-r3 are already preserved by the AAPCS, and so is r12-r16 (which contains the SP, LR, PC, xPSRetc..)
            // However, these are saved on the stack of the current thread, 
            // so the stack pointer must be saved in the Thread Control Block,
            //
            // First the address of OS_current is needed. But this was already loaded in R0!
            // Then I guess we can store the sp, with one STR instruction, with an offset based on the struct.
            // Turns out, there is NO offset in the OS_thread struct. Great!
            __asm("     STR              sp,[r0]");
            
           // IGNORE? B             0x0000076E
    // END OF PART WHERE CONTEXT IS BEING SAVED.
// PLACE LABEL HERE SO THE IF STATEMENT BODY CAN BE SKIPPED OVER IN CASE THE CURRENT THREAD WAS ZERO
// Label must be on assembly line.
            // Now the context for the next thread can be restored.
            // Remember that the context is stored on the stack of this thread.
            // So first we need the stack pointer of OS_next
            // And again, we first need the address of OS_curr pointer.
            // Let's load it in r1, because we might need r0 still.
//    75:     sp = OS_next->sp; // OS_next does not have to be checked for being a null pointer? 
__asm("PendSV_restore:     LDR   r1,=OS_next");
            // Then load the actual addres to the thread control block
            __asm("     LDR              r1,[r1]");
            // At this address with an offset of ZERO we can find the stack pointer of this next thread.
            // Let's load it into the stack pointer register.
            __asm("     LDR              sp,[r1]");
//    76:      // Now the registers r4-r11 must be restored. The order is reversed because r4-r11 are pushed on this thread's stack 
            // POP POP! Restore registers r4-r11.
            // The order of storing, loading the stack pointer and pushing/popping the registers is obviously reversed.
            __asm("     POP             {r4-r11}");
//    77:     OS_curr = OS_next;
            // Now the context is switch, we the next thread becomes the current thread.
            // CAREFUL THOUGH! Don't mix up the pointer to the address of the Thread Control Block, and the address to the thread control block.
            // We must put the ADDRESS to the Thread Control Block into the OS_curr POINTER variable.
            // So first we must load the ADDRESS TO THE POINTER! We cannot re-use R0 for this!
            __asm("     LDR              r0,=OS_curr");
            // Secondly, we require the address of the next thread control, and put it in this pointer.
            // This value in R1 can be re-used, as it contains the address to Thread Control Block OS_next
            //__asm("LDR              r1,=OS_next");
            // In the store command, the first argument R1 holds the VALUE to be stored. It will be stored at the ADDRESS r0.
            // So again:    r0 currently holds the ADDRESS to a pointer to OS_Thread.
            //              r1 holds a VALUE, which is the pointer to an OS_Thread.
            __asm("     STR              r1,[r0]");
            // DONE! Now the interupts can be re-enabled.
//    80:     __enable_irq();
            __asm("     CPSIE            I"); //Enable interupts.
            // And we can branch back with help of the just restored link-register!
//    81: }
            __asm("     BX               lr");
}
