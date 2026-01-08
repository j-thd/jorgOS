/* Board Support Package */

#include "runtime_environment.h"
#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_led.h"
#include "bsp_switch.h"
#include "jorgOS.h"
#include "jEventQueueThread.h"
#include "jAssert.h"
#include "uart.h"
#include "qei.h"

#include <stdio.h>

// The jorgOS-specific thread to handle events with.
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

    // Initialize the timers
    BSP_timer_init();

    // Initialize QEI
    QEI_0_init();

}

/// @brief Tick function for any non-OS but board-related updates, such as
/// debouncing buttons. The only OS-related feature in this function is the
/// EventQueueThread to which events will be posted.
void BSP_tick(){
    J_REQUIRE_IN_CRIT_SEC;
    BSP_switch_tick(BSP_EQT_thread);
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

void OS_onStartup(void){
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}
/// JTEST Callbacks

// A guard is strictly not necessary, but these functions should only be used
// when running a test on the target

#ifdef JTEST_TARGET
void JTEST_init_target(){
    // Function called by JTEST to allow the target to define how it must be
    // initialized.
        BSP_init();
        //BSP_LED_blue_on();
}



void JTEST_target_flush_buffer(){
    JSM_transmit_buffer();
}

void JTEST_target_shutdown(){
    //Function called by JTEST to allow the target to define how it must be
    //stopped.
    
    // We need to transmit the last things in the buffer of the serial monitor,
    // so we can see the results.
    JSM_PRINTF("Shutting down target...\n");
    JSM_transmit_buffer();
    while(1){
        // Sleep in low-power mode.
        __WFI();
    }
}
#endif