#include "jorgOS.h"
#include "jAssert.h"
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
uint32_t OS_ready_set; // A 32-bit mask for ready threads. Idle is always ready, and not in this bitmask
uint32_t OS_delayed_set; // A 32-bit mask holding the set of delayed threads. Threads can be delayed and NOT ready due to mutexes or stuff like that.

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
    
    // The idle thread is created with priority 0. Higher means more priority.
    OS_Thread_start(&idle_thread,
        0U, // Priority
        main_idle_thread,
        idle_thread_stack,
        sizeof(idle_thread_stack));
}

void OS_delay(uint32_t ticks_delay){
    __disable_irq(); // Race condition galore here. OS_current cannot switch in the meantime, and neither can be the readyset bits be altered.

    // OS_delay may not be called in the idle thread.
    J_REQUIRE(OS_curr != OS_thread_list[0]);
    
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
    __enable_irq();
}

//This function should always be called in a critical section.
void OS_tick(void){
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
        
        priority_bit = thread->priority - 1U;
        // If the thread timeout hits 0, the thread is no longer delayed and ready to run
        if (thread->timeout == 0U){
            OS_ready_set |= 1U << priority_bit;
            OS_delayed_set &= ~ priority_bit;
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
    // Priority based-scheduling. Highest priority runs first. 0 is the idle thread.
    // Normal threads range from 1-32.
    OS_next = OS_thread_list[THREAD_WITH_HIGHEST_PRIORITY(OS_ready_set)];
    J_ASSERT(OS_next != (void *)0); // The thread must exist

    // Trigger the PendSV interupt handler when the next thread differs from the current one
    if (OS_next != OS_curr) {
        SCB->ICSR |=  SCB_ICSR_PENDSVSET_Msk; // Set the PendSV bit in the INTCTRL register of the SCB.
    }
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
    *(--(sp)) = (uint32_t)threadHandler; // Pointer to main_blinky1 in PC
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
    J_ASSERT(priority < MAX_NUMBER_OF_THREADS); // No semi-colon here as it is an if-else statement in disguise {}
    J_ASSERT(OS_thread_list[priority] == (void *)0U); // Should be a null pointer if no thread is assigned yet at this priority
    
    OS_thread_list[priority] = me;
    me->priority = priority; // The  TCB must know about its own priority, finding it in a list is a bit idea.
    //Set the thread to be ready to run, but only if it is not the IDLE thread
    if (priority > 0U){
        OS_ready_set |= (1U << (priority-1U) );
    }

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
