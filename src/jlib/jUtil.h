/*
    A file for storing miscellaneous utility functions
*/
#include <stdint.h>
#include <stdlib.h>


/*   MACROS
*/

// Determining the address of a struct member at compile-time
#define J_STRUCT_MEMBER_ADDRESS(struct_, struct_type_, member_) ( (uintptr_t*)((uintptr_t)struct_ + offsetof(struct_type_, member_)) )

// Repeating expressions for array initialization. Expand as necessary

// Concat function to automatically pick the right one.

#define J_ARRAY_N_X_INTERNAL(n,x) J_ARRAY_ ## n ## (x)
#define J_ARRAY_N_X(n,x) J_ARRAY_N_X_INTERNAL(n,x)

#define J_ARRAY_1(x) x
#define J_ARRAY_2(x) J_ARRAY_1(x), J_ARRAY_1(x)

#define J_ARRAY_3(x) J_ARRAY_2(x), J_ARRAY_1(x)
#define J_ARRAY_4(x) J_ARRAY_2(x), J_ARRAY_2(x)

#define J_ARRAY_5(x) J_ARRAY_4(x), J_ARRAY_1(x)
#define J_ARRAY_6(x) J_ARRAY_4(x), J_ARRAY_2(x)
#define J_ARRAY_7(x) J_ARRAY_4(x), J_ARRAY_3(x)
#define J_ARRAY_8(x) J_ARRAY_4(x), J_ARRAY_4(x)

#define J_ARRAY_9(x)    J_ARRAY_8(x), J_ARRAY_1(x)
#define J_ARRAY_10(x)   J_ARRAY_8(x), J_ARRAY_2(x)
#define J_ARRAY_11(x)   J_ARRAY_8(x), J_ARRAY_3(x)
#define J_ARRAY_12(x)   J_ARRAY_8(x), J_ARRAY_4(x)
#define J_ARRAY_13(x)   J_ARRAY_8(x), J_ARRAY_5(x)
#define J_ARRAY_14(x)   J_ARRAY_8(x), J_ARRAY_6(x)
#define J_ARRAY_15(x)   J_ARRAY_8(x), J_ARRAY_7(x)
#define J_ARRAY_16(x)   J_ARRAY_8(x), J_ARRAY_8(x)

#define J_ARRAY_32(x) J_ARRAY_16(x), J_ARRAY_16(x)


