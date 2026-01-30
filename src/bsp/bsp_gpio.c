#include "bsp_gpio.h"



#include "runtime_environment.h"

/*
    Current GPIO pin assignments.
    Also trying to track where code is initialized.
    Enabling clocks and AHB is all done here, though.

    // GPIO_D
    - D1 LED 1 GREEN (done in BSP_LED)
    // GPIO_E
    - E4 LED 1 BLUE (done in BSP_LED)
    - E5 LED 1 RED (done in BSP_LED)

    // GPIO_F
    - F0 SWITCH_2 (done in BSP_SWITCH)
    - F1 LED 0 RED (done in BSP_LED)
    - F2 LED 0 BLUE (done in BSP_LED)
    - F3 LED 0 GREEN (done in BSP_LED)
    - F4 SWITCH_1 (done in BSP_SWITCH.H)
*/


void BSP_GPIO_init(void){
    SYSCTL->RCGC2 |= GPIO_D | GPIO_E | GPIO_F; // Enable clocks
    SYSCTL->GPIOHBCTL  |= GPIO_D | GPIO_E | GPIO_F; // Enable AHB for GPIOF
    
}