/*
    A file for storing miscellaneous utility functions
*/
#include <stdint.h>
#include <stdlib.h>


/*   MACROS
*/

// Determining the address of a struct member at compile-time
#define J_STRUCT_MEMBER_ADDRESS(struct_, struct_type_, member_) ( (uintptr_t*)((uintptr_t)struct_ + offsetof(struct_type_, member_)) )