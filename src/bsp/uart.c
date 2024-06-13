#include "runtime_environment.h"
#include "jAssert.h"
#include "uart.h"


void UART0_init(uint32_t baud_rate){
    /* UART implementation for the TM4C123GH6PM
    * Implementation follows instructions from Section 14.4 (p. 902) of the datasheet.
    */
    // 1. Enable UART0 module using RCGCUART (clock gating)
    SYSCTL->RCGCUART |= UART0_BIT;  
    // 2. Enable the clock to the approprioate GPIO module via RCGCGPIO
    // There appears to be no regularity in which GPIO must be enabled.
    // It is perhaps best to hardcode this.
    SYSCTL->RCGCGPIO |= UART0_GPIO;
    SYSCTL->GPIOHBCTL |= UART0_GPIO;
    //3. Set the UART GPIO AFSEL bits (alternative function select) for the
    //   correct module
    GPIOA_AHB->AFSEL |= UART0_RX_TX_PINS;
    GPIOA_AHB->PCTL &= ~0U;
    GPIOA_AHB->PCTL |= UART0_GPIOPCTL_RX_TX;
    // 3.2? (Some registers don't seem to be set by default, so needs to be done
    // anyway)
    GPIOA_AHB->DEN |= UART0_RX_TX_PINS; 

    //4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see page 673 and page 681).
    // Found the registers, but honestly don't see what "mode" this is referring to.

    //5. 1 Disable the UART before changing the following settings
    UART0->CTL &= ~UARTCTL_UARTEN;
    // 5.2-3 The baud-rate generator depends on the clock used for UART, which is
    // the System Core Clock by Default.
    // Clock Divisor is is 16 if HSE is NOT set in the UARTCTL register.
    // Otherwise it is 8. Let's hardcode for HSE disabled for now.
    // The integer part of the BRD
    // Update the SystemCoreClock variable provided by CMSIS. This is probably
    // only necessary after the clock is changed or after system-reset, but
    // let's do it just here to be sure.
    SystemCoreClockUpdate();
    uint16_t DIVINT = SystemCoreClock / (16 * baud_rate);
    UART0->IBRD = DIVINT;
    // The decimal part of the BRD
    float BRDF = ((float)SystemCoreClock / (16 * baud_rate)) - (float)DIVINT;
    uint8_t DIVFRAC = BRDF * 64 + 0.5;
    J_ASSERT(DIVFRAC <= MAX_DIVFRAC);
    UART0->FBRD = DIVFRAC;

    // 5.4 Write the desired serial parameters to UARTLCRH. This must always
    // happen after the baud-rate is updated even if nothing changes.
    // For now let's only set 8 bits word length
    UART0->LCRH = UART_WLEN_8BITS;

    // 5.5 Configure the UART clock source by writing to the UARTCC register.
    UART0->CC = UARTCC_CS_SYSCLOCK;

    // 5.6 Optional part. Ignoring for now

    // 5.7 Enable the UART
    UART0->CTL |= UARTCTL_UARTEN;

}

void UART_write(UART0_Type* UARTn, uint8_t c){
    // Loop until the UART is no longer busy, then send another character.
    while(UARTn->FR & UARTFR_BUSY ){

    }
    UARTn->DR = c; // Put the character in the data register.

}