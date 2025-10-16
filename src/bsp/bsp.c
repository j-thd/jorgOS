/* Board Support Package */

#include "runtime_environment.h"
#include "bsp.h"
#include "bsp_led.h"
#include "jorgOS.h"
#include "jEventQueueThread.h"
#include "jAssert.h"
#include "uart.h"
#include "jtest.h"

#include <stdio.h>

// The jorgOS-specific function to handle events with.
OS_EventQueue_Thread * BSP_EQT_thread;

void BSP_init(void){
    SYSCTL->RCGC2 |= (1U << 5); // Enable clock for GPIOF
    SYSCTL->GPIOHBCTL  |= (1U <<5); // Enable AHB for GPIOF
    GPIOF_AHB->DIR |= ( LED_RED | LED_BLUE | LED_GREEN ); //Set Pin direction on pin 1, 2, 3 as output.
    GPIOF_AHB->DEN |= ( LED_RED | LED_BLUE | LED_GREEN ); // Digital Enable 

    // Initialize PWM to drive the LEDs
    BSP_LED_PWM_init();

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

    //BSP_LED_red_on();

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
            J_Event e = {BUTTON_1_DEPRESSED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 1 pressed\n");
        }
        else { // The switch was just released
            J_Event e = {BUTTON_1_RELEASED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 1 released\n");
        }
    }

    if ( temp & SWITCH_2 ){ // Switch 1 state changed
        if (buttons.depressed & SWITCH_2){ // Switch 2 was pressed this tick
            J_Event e = {BUTTON_2_DEPRESSED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 2 pressed\n");
        }
        else { // The switch was just released
            J_Event e = {BUTTON_2_RELEASED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 2 released\n");
        }
    }

    //BSP_LED_red_off();
    // Update the LED
    //BSP_LED_update();
}

void BSP_register_EQT_thread(OS_EventQueue_Thread * eqt_thread){
    J_REQUIRE(eqt_thread != (void *)0);
    BSP_EQT_thread = eqt_thread;
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

    J_CRIT_SEC_START();
    OS_tick();
    BSP_tick();
    
    OS_schedule();
    J_CRIT_SEC_END();
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

// LED COLOURS THROUGH PWM
void BSP_LED_PWM_init(void){
    // This is partially based on the TM4C datasheet page 1239. Register names
    // seem to be deprecated in this manual.
    SYSCTL->RCGCPWM |= (1U << 1); // Enable clock for PWM1
    // Set the pins to alternative functions so it can be set respond to PWM
    // signals
    GPIOF_AHB->AFSEL |= (LED_RED | LED_GREEN | LED_BLUE);
    //Set the Port Control to the PWM signals.
    GPIOF_AHB->PCTL |= (LED_RED_PMC | LED_GREEN_PMC | LED_BLUE_PMC);
    // Turn on the clock divisor for PWM
    SYSCTL->RCC |= 1U << RCC_USEPWMDIV;
    // Set the divisor to 2 with 0x0, realizing it is 0x7 by default.
    // So I think it must be cleared first with 3 1-bits (0x7), if you want to
    // set anything but 0x0.
    SYSCTL->RCC &= !(0x7 << RCC_PWMDIV);
    
    // This just puts the control for block PWM1 Block 2 to the default
    // settings, which mostly consists bitfields for it being enabled and how it
    // should deal with updates to particular values (synchronization). Also,
    // something about fault handling.
    // PWM Block 1 Generator 2 b is for Pin F1 (red LED)
    PWM1->_2_CTL = 0x0;
    // PWM Block 1 Generator 3 a & b is for Pin F2 & F3 (blue & green LED)
    PWM1->_3_CTL = 0x0;
    
    // RED LED (Generator 2 pwm b)
    // Drive the pwmB signal high on LOAD value and low on CMP B value (when
    // counting down)
    PWM1->_2_GENB = ( ACTLOAD_PWM_x_HIGH | ACTCMPBD_PWM_x_LOW );
    // Blue LED (Generator 3 pwm A)
    // Drive the pwmA signal high on LOAD value and low on CMP A value (when
    // counting down)
    PWM1->_3_GENA = ( ACTLOAD_PWM_x_HIGH | ACTCMPAD_PWM_x_LOW );
    // Green LED
    // Drive the pwmB signal high on LOAD value and low on CMP B value (when
    // counting down)
    PWM1->_3_GENB = ( ACTLOAD_PWM_x_HIGH | ACTCMPBD_PWM_x_LOW );

    // Provided that the clock is 10 MHz (System clock divided by 2 with PWMDIV
    // 399 for 400 ticks per period for 40 microseconds resulting in 25 kHz
    PWM1->_2_LOAD = 255; // This value drives pwm high according to GENx settings
    PWM1->_3_LOAD = 255;
    // 99 for 75% duty cycle (100/400). 
    PWM1->_2_CMPB = 220; // This value drives 2pwmB low // RED
    PWM1->_3_CMPA = 220; // This value drives 3pwmA low // BLUE
    PWM1->_3_CMPB = 220; // This value drives 3pwmB low // GREEN

    PWM1->_2_CTL = 0x1; // Enable PWM 1 Generator 2
    PWM1->_3_CTL = 0x1; // Enable PWM 1 Generator 3
    PWM1->ENABLE = 0x7 << 5; // Enable pwm5(3b), 6(4a) and 7(4b)

    while (1)
    {
        // Loop around to test the LED behaviour :)
    }

}

void JTEST_init_target(){
    // Function called by JTEST to allow the target to define how it must be
    // initialized.
        BSP_init();
}