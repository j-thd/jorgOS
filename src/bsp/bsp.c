/* Board Support Package */

#include "runtime_environment.h"
#include "bsp.h"
#include "jorgOS.h"
#include "jAssert.h"
#include "uart.h"

#include <stdio.h>

// Arrays to store callback functions for buttons
// BSP_Button_Depressed_Handler btn_depressed_handlers[MAX_REGISTERED_BUTTONS];
// BSP_Button_Released_Handler btn_release_handlers[MAX_REGISTERED_BUTTONS];


void BSP_init(void){
    SYSCTL->RCGC2 |= (1U << 5); // Enable clock for GPIOF
    SYSCTL->GPIOHBCTL  |= (1U <<5); // Enable AHB for GPIOF
    GPIOF_AHB->DIR |= ( LED_RED | LED_BLUE | LED_GREEN ); //Set Pin direction on pin 1, 2, 3 as output.
    GPIOF_AHB->DEN |= ( LED_RED | LED_BLUE | LED_GREEN ); // Digital Enable 

    // Initializing switches
    BSP_SWITCH_1_init();
    BSP_SWITCH_2_init();

    // Enable UART0 which communicates over the USB by default
    UART0_init(9600);
    // Use UART0 for the jorgOS Serial Monitor (using printf)
    #ifdef JSM_ENABLE
    JSM_init(UART0);
    printf("JSM Logging enabled\n");
    JSM_transmit_buffer();
    #endif //JSM_ENABLE



}

/// @brief Tick function for any non-OS but board-related updates, such as
/// debouncing buttons.
void BSP_tick(){
    J_REQUIRE_IN_CRIT_SEC;
    static struct DebouncedButtons {
        uint32_t depressed;
        uint32_t previous;
    } buttons = {0U, 0U};

    uint32_t current;
    uint32_t temp;

    current = ~GPIOF_AHB->DATA_BITS[SWITCH_1 | SWITCH_2];
    temp = buttons.depressed;
    buttons.depressed |= (buttons.previous & current); // Set depressed buttons. (down two ticks in a row)
    buttons.depressed &= (buttons.previous | current); // Set released buttons (up two ticks in a row)
    buttons.previous = current;

    // Now check if the depressed state changed.
    temp ^= buttons.depressed;

    if ( temp & SWITCH_1 ){ // Switch 1 state changed
        if (buttons.depressed & SWITCH_1){ // Switch 1 was pressed this tick
            JSM_PRINTF("Switch 1 pressed\n");
            // TODO: Event-queue put goes here
        }
        else { // The switch was just released
            JSM_PRINTF("Switch 1 released\n");
            // TODO: Event-queue put goes here
        }
    }

    if ( temp & SWITCH_2 ){ // Switch 1 state changed
        if (buttons.depressed & SWITCH_2){ // Switch 1 was pressed this tick
            JSM_PRINTF("Switch 2 pressed\n");
            // TODO: Event-queue put goes here
        }
        else { // The switch was just released
            JSM_PRINTF("Switch 2 released\n");
            // TODO: Event-queue put goes here
        }
    }


}

__attribute__((noreturn)) void J_on_assert_failed (char const *file, int line) {
    JSM_PRINTF("[ASSERTION FAILED] File %s on line %u\n", file, line);
    
    // Only transmit the buffer before reset if JSM is enabled.
    #ifdef JSM_ENABLE
        // The buffer must be transmitted now, as it normally only happens in the
        // idle thread.
        JSM_transmit_buffer();
        // The UART must finish sending the last character, and JSM_transmit_buffer
        // returns before that is done. Only then reset the MCU.
        UART_delay_until_not_busy(UART0);
    #endif // JSM_ENABLE
    NVIC_SystemReset();
    while(1){
        // Shuts up faulty warning about not returning. The compiler does not understand NVIC_SYSTEM_reset
        
    }
}

void SysTick_Handler(void){
    //GPIOF_AHB->DATA_BITS[(LED_GREEN)] ^= LED_GREEN;

    __disable_irq();
    OS_tick();
    BSP_tick();
    OS_schedule();
    __enable_irq();
}

void OS_onIdle(void){
    #ifdef JSM_ENABLE
    //GPIOF_AHB->DATA_BITS[(LED_RED)] = LED_RED;
    JSM_transmit_buffer();
    //GPIOF_AHB->DATA_BITS[(LED_RED)] = 0;
    #endif // JSM_ENABLE
    
    __WFI();
}

void OS_onStartup(void){
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}

// LED

void BSP_LED_red_on(void){
    GPIOF_AHB->DATA_BITS[(LED_RED)] = LED_RED;
}

void BSP_LED_red_off(void){
    GPIOF_AHB->DATA_BITS[(LED_RED)] = 0;
}

void BSP_LED_green_on(void){
    GPIOF_AHB->DATA_BITS[(LED_GREEN)] = LED_GREEN;
}

void BSP_LED_green_off(void){
    GPIOF_AHB->DATA_BITS[(LED_GREEN)] = 0;
}

void BSP_LED_blue_on(void){
    GPIOF_AHB->DATA_BITS[(LED_BLUE)] = LED_BLUE;
}

void BSP_LED_blue_off(void){
    GPIOF_AHB->DATA_BITS[(LED_BLUE)] = 0;
}

// Switches
void BSP_SWITCH_1_init(void){
    // Set the switch as an input (0), enable digital
    GPIOF_AHB->DIR &= ~SWITCH_1;
    GPIOF_AHB->DEN |= SWITCH_1;

    // Pull-up resistor
    GPIOF_AHB->PUR |= SWITCH_1;


}
void BSP_SWITCH_2_init(void){
    // The registers for Switch 2 are protected, and must be unlocked first.
    GPIOF_AHB->LOCK = 0x4C4F434B;
    GPIOF_AHB->CR |= SWITCH_2;
    // Set the switch as an input (0), enable digital
    GPIOF_AHB->DIR &= ~SWITCH_2;
    GPIOF_AHB->DEN |= SWITCH_2;

    // Pull-up resistor
    GPIOF_AHB->PUR |= SWITCH_2;

    // Restore and relock the commit register
    GPIOF_AHB->CR &= ~SWITCH_2;
    GPIOF_AHB->LOCK = 0U;

}