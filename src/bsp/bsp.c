/* Board Support Package */

#include "runtime_environment.h"
#include "bsp.h"
#include "jorgOS.h"
#include "uart.h"

#include <stdio.h>

void BSP_init(void){
    SYSCTL->RCGC2 |= (1U << 5); // Enable clock for GPIOF
    SYSCTL->GPIOHBCTL  |= (1U <<5); // Enable AHB for GPIOF
    GPIOF_AHB->DIR |= ( LED_RED | LED_BLUE | LED_GREEN ); //Set Pin direction on pin 1, 2, 3 as output.
    GPIOF_AHB->DEN |= ( LED_RED | LED_BLUE | LED_GREEN ); // Digital Enable 

    // Enable UART0 which communicates over the USB by default
    UART0_init(9600);
    // Use UART0 for the jorgOS Serial Monitor (using printf)
    #ifdef JSM_ENABLE
    JSM_init(UART0);
    printf("JSM Logging enabled\n");
    JSM_transmit_buffer();
    #endif //JSM_ENABLE



}

__attribute__((noreturn)) void J_on_assert_failed (char const *file, int line) {
    // UART_write(UART0, 'E');
    // UART_write(UART0, 'r');
    // UART_write(UART0, 'r');
    // UART_write(UART0, 'o');
    // UART_write(UART0, 'r');
    // UART_write(UART0, '!');
    // UART_write(UART0, '\r');
    // UART_write(UART0, '\n');

    JSM_PRINTF("[ASSERTION FAILED] File %s on line %u\n", file, line);
    JSM_transmit_buffer();
    UART_delay_until_not_busy(UART0);
    // volatile int i = 1000;
    // while (i>0){
    //     i--;
    // }
    //UART_disable(UART0);
    NVIC_SystemReset();
    while(1){
        // Shuts up faulty warning about not returning. The compiler does not understand NVIC_SYSTEM_reset
        
    }
}

void SysTick_Handler(void){
    //GPIOF_AHB->DATA_BITS[(LED_GREEN)] ^= LED_GREEN;

    __disable_irq();
    OS_tick();
    OS_schedule();
    __enable_irq();
}

void OS_onIdle(void){
    #ifdef JSM_ENABLE
    GPIOF_AHB->DATA_BITS[(LED_RED)] = LED_RED;
    JSM_transmit_buffer();
    GPIOF_AHB->DATA_BITS[(LED_RED)] = 0;
    #endif // JSM_ENABLE
    
    __WFI();
}

void OS_onStartup(void){
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}