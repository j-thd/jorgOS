#ifndef __UART_H__
#define __UART_H__

/* UART implementation for the TM4C123GH6PM
 * Implementation follows instructions from Section 14.4 (p. 902) of the datasheet.
 */

#define MAX_INDEX_UART_MODULES (7U) // Highest available index of UART modules. (i.e. 8 modules, highest index is 7.)

// Define on which GPIO the UART lives
#define UART0_BIT (1U << 0)
#define UART0_GPIO (1U << 0)
#define UART0_AFSEL_RX_TX (0x3)
#define UART0_RX_TX_PINS (0x3)
#define UART0_GPIOPCTL_RX_TX (0x3)

// UART Control (UARTCTL)
#define UARTCTL_UARTEN (1U) // Enable UART
#define UART_RXE (1U << 9) // Receive enable
#define UART_TXE (1U << 8) // Transmit enable

// UART Fractional Baud-Rate Divisor (UARTFBRD)
#define MAX_DIVFRAC ((1U << 6U) -1U ) // Only 6 bits allowed

// UART Line Control (UARTLCRH)
#define UART_PEN (1U << 1)
#define UART_WLEN_8BITS (0x3 << 5)

// UART Clock Configuration (UARTCC)
#define UARTCC_CS_SYSCLOCK (0x0)

// UART Flag (UARTFR)
#define UARTFR_BUSY (1U << 3)

void UART0_init(uint32_t baud_rate);
void UART_write(UART0_Type* UARTn, uint8_t c);
void UART_disable(UART0_Type* UARTn);
void UART_delay_until_not_busy(UART0_Type* UARTn);

#endif // __UART_H__