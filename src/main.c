#include <stdint.h>

#include "TM4C123.h"
#include "bsp.h"
#include "jorgOS.h"
#include "jAssert.h"
#include "runtime_environment.h"



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

int main(void) {    
    
    BSP_init();

    OS_init();

    OS_Thread_start(&blinky_blue_thread, PRIO_BLINKY_BLUE, &blinky_blue, 
        blinky_blue_stack, sizeof(blinky_blue_stack));
    
    OS_Thread_start(&blinky_green_thread, PRIO_BLINKY_GREEN, &blinky_green, 
        blinky_green_stack, sizeof(blinky_green_stack));

    OS_run();

    // After the first thread is scheduled, OS_run() should never return.
    
    J_ERROR();

    
    //return 0;
}