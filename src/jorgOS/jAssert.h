#ifndef __JASSERT_H__
#define __JASSERT_H__

#include <stdint.h>



// The base assert macro
#define J_ASSERT(expr_) \
    (expr_) ? (void *)0 : J_assert_failed(__FILE__, __LINE__)

#define J_REQUIRE(expr_) J_ASSERT(expr_) // Pre-condition
#define J_ENSURE(expr_) J_ASSERT(expr_) // Post-condition

#define J_ERROR() do {\
        J_assert_failed(__FILE__, __LINE__);\
    } while(0) // Always an error, code should not be reached

#define J_DEFAULT_STACK_FILLER 0xDEADBEEF
#define J_ASSERT_STACK_INTEGRITY(stack_, stack_size_) J_assert_stack_integrity(stack_,stack_size_)

//Dealing with assertions
void J_assert_failed (char const *file, int line);
//Callback function that must be implemented by BSP package.
void J_on_assert_failed(char const *file, int line);

// Stack integrity asserts
void J_assert_stack_integrity(uint32_t * stack, uint16_t stack_size);

#endif // __JASSERT_H__