#include <stdint.h>

#include "TM4C123.h"
#include "bsp.h"
#include "jorgOS.h"
#include "jAssert.h"
#include "jSema.h"
#include "runtime_environment.h"


// Semaphore test
J_sema sema_test;


#define BLINKY_BLUE_STACKSIZE 100U
#define PRIO_BLINKY_BLUE 5U
uint32_t blinky_blue_stack[BLINKY_BLUE_STACKSIZE];
OS_Thread blinky_blue_thread;

void blinky_blue(void){
    JSM_PRINTF("Blinky blue thread started!\n");
    while(1) {
        J_ASSERT_STACK_INTEGRITY(blinky_blue_stack, BLINKY_BLUE_STACKSIZE);
        GPIOF_AHB->DATA_BITS[(LED_BLUE)] = LED_BLUE;    
        OS_delay(3*BSP_TICKS_PER_SEC); 
        GPIOF_AHB->DATA_BITS[(LED_BLUE)] = 0;
        OS_delay(1*BSP_TICKS_PER_SEC); 
    }
}

#define BLINKY_GREEN_STACKSIZE 100U
#define PRIO_BLINKY_GREEN 2U
uint32_t blinky_green_stack[BLINKY_GREEN_STACKSIZE];
OS_Thread blinky_green_thread;

void blinky_green(void){
    JSM_PRINTF("Blinky green thread started!\n");
    while(1) {
        J_ASSERT_STACK_INTEGRITY(blinky_green_stack, BLINKY_GREEN_STACKSIZE);
        GPIOF_AHB->DATA_BITS[(LED_GREEN)] = LED_GREEN;    
        OS_delay(BSP_TICKS_PER_SEC / 4U); 
        GPIOF_AHB->DATA_BITS[(LED_GREEN)] = 0;
        OS_delay(BSP_TICKS_PER_SEC / 4U); 
    }
}

#define SEMA_RED_STACKSIZE 100U
#define PRIO_SEMA_RED 10U
uint32_t sema_red_stack[SEMA_RED_STACKSIZE];
OS_Thread sema_red_thread;

// Thread to see if semaphore works in response to signals from another thread.
void sema_red(void){
    JSM_PRINTF("Sema red thread started. \n");
    while (1) {
        J_ASSERT_STACK_INTEGRITY(sema_red_stack, SEMA_RED_STACKSIZE);
        // Blink briefly after the semaphore has been signaled.
        JSM_PRINTF("Sema_red waiting... \n");
        GPIOF_AHB->DATA_BITS[(LED_RED)] = LED_RED;
        J_sema_wait(&sema_test);
        JSM_PRINTF("Sema_red thread continues. \n");
        
        OS_delay(BSP_TICKS_PER_SEC / 2U);
        GPIOF_AHB->DATA_BITS[(LED_RED)] = 0;

    }
}

#define SEMA_TEST_SIGNAL_STACKSIZE 100U
#define PRIO_SEMA_TEST_SIGNAL 9U
uint32_t sema_test_signal_stack[SEMA_TEST_SIGNAL_STACKSIZE];
OS_Thread sema_test_signal_thread;

// Dummy thread to signal a semaphore.
void sema_test_signal(void){
    JSM_PRINTF("Sema test signal thread started. \n");
    while(1){
        J_ASSERT_STACK_INTEGRITY(sema_test_signal_stack, SEMA_TEST_SIGNAL_STACKSIZE);
        J_sema_signal(&sema_test);
        JSM_PRINTF("Sema test signaled... \n");
        OS_delay(BSP_TICKS_PER_SEC*5U); // Wait longer than the sema_red threads sleeps.

    }
}

int main(void) {    
    
    BSP_init();

    OS_init();

    // Initialize semaphores
    J_sema_init(&sema_test, 0, 1);

    OS_Thread_start(&blinky_blue_thread, PRIO_BLINKY_BLUE, &blinky_blue, 
        blinky_blue_stack, sizeof(blinky_blue_stack));
    
    OS_Thread_start(&blinky_green_thread, PRIO_BLINKY_GREEN, &blinky_green, 
        blinky_green_stack, sizeof(blinky_green_stack));

    OS_Thread_start(&sema_red_thread, PRIO_SEMA_RED, &sema_red, 
        sema_red_stack, sizeof(sema_red_stack));

    OS_Thread_start(&sema_test_signal_thread, PRIO_SEMA_TEST_SIGNAL, &sema_test_signal, 
        sema_test_signal_stack, sizeof(sema_test_signal_stack));

    OS_run();

    // After the first thread is scheduled, OS_run() should never return.
    
    J_ERROR();

    
    //return 0;
}