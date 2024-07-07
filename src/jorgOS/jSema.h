#ifndef __JSEMA_H__
#define __JSEMA_H__

#include <stdint.h>
#include <stdbool.h>

//# define MAX_NUMBER_J_SEMA
// The J_sema semaphore object.
typedef struct {
    uint8_t count; // When 0 the threads waiting on the semaphore must wait
    uint8_t max_count; // Maximum amount of resources sema can keep track off
    
} J_sema;

//J_sema J_sema_register[MAX_NUMBER_J_SEMA];

void J_sema_init(J_sema *sema, uint8_t initial_count, uint8_t max_count);
void J_sema_signal(J_sema *sema);
void J_sema_wait(J_sema *sema);
bool J_sema_check_for_ready(J_sema *sema);
#endif //__JSEMA_H__