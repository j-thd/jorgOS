#ifndef __JASSERT_H__
#define __JASSERT_H__

#include <stdint.h>
#include <stdbool.h>

#include "jorgOS.h"
#include "jCritSec.h"




// The base assert macro
#define J_ASSERT(expr_) \
    (expr_) ? (void *)0 : J_assert_failed(__FILE__, __LINE__)

#define J_REQUIRE(expr_) J_ASSERT(expr_) // Pre-condition
#define J_ENSURE(expr_) J_ASSERT(expr_) // Post-condition
#define J_REQUIRE_IN_CRIT_SEC J_ASSERT(__get_PRIMASK() == true)

#define J_ERROR() do {\
        J_assert_failed(__FILE__, __LINE__);\
    } while(0) // Always an error, code should not be reached

// Stack integrity
#define J_DEFAULT_STACK_FILLER 0xDEADBEEF
#define J_ASSERT_STACK_INTEGRITY(stack_, stack_size_) J_assert_stack_integrity(stack_,stack_size_)

// Thread integrity
#define J_REQUIRE_ZERO_OR_ONE_BLOCKING_REASON(thread_) J_REQUIRE( ! (thread_->blocking_mutex != (void *)0 && thread_->blocking_sema != (void *)0 ))
#define J_REQUIRE_ONE_BLOCKING_REASON(thread_) J_REQUIRE(  (thread_->blocking_mutex != (void *)0) != (thread_->blocking_sema != (void *)0 ))
#define J_ASSERT_TCB_INTEGRITY J_ASSERT(OS_assert_TCB_integrity())

//Dealing with assertions
void J_assert_failed (char const *file, int line);
//Callback function that must be implemented by BSP package.
void J_on_assert_failed(char const *file, int line);

// Stack integrity asserts
void J_assert_stack_integrity(uint32_t * stack, uint16_t stack_size);

#endif // __JASSERT_H__