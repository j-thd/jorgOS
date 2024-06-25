#include <stdint.h>

#include "jAssert.h"



__attribute__((noreturn)) void J_assert_failed(char const *file, int line) {
    //Callback function must be implemented on BSP
    J_on_assert_failed(file, line);
}

/// @brief Function that uses some assembly to check if the stack pointer is
/// still within bounds of the stack of a thread.
/// @param stack uint32_t * - Pointer to the stack 
/// @param stack_size uint16_t - Size of the stack (4 bytes = size 1)
void J_assert_stack_integrity(uint32_t * stack, uint16_t stack_size){
    /*
     *    STACK POINTER BOUNDS CHECK
    */
    
    //Somehow obtain the stack-pointer and check if it is in bounds of the
    //stack.
    uint32_t * current_sp;
    __asm("MOV %[csp], sp"
            : [csp] "=r" (current_sp));

    // On the ARM Cortex-M4f the stack pointer points to the last stored
    // item. When a new item is stored the stack pointer is decreased, and
    // then a new item is stored. (Section 2.3.2 of the TM4C123GH6PM manual)

    // This means that when the stack is full, the pointer is exactly
    // &stack[stack_size].
    
    // When the stack is empty the stack pointer is thus one above the stack.

    // The stack pointer grows down so, so the allocated uint32_t[stack_size]
    // stack has the bottom of the stack at stack[0]. The top of the stack is at
    // stack[stack_size.]

    // Check the bottom of the stack
    J_ASSERT(current_sp >= stack);
    // Check the top of the stack
    J_ASSERT(current_sp <= (stack + stack_size + 1));

    /*
     *    STACK OVERFLOW CHECK
    */

   // J_DEFAULT_STACK_FILLER was used to fill empty places in the stack when the
   // stack was created. The last available place in the stack can be used to
   // check how much the stack has grown. If the filler value is no longer
   // there, the stack has overflown.

   // The grows from top to bottom, so the last value should be at the lowest
   // point of memory.

   J_ASSERT(stack[1] == J_DEFAULT_STACK_FILLER);
}