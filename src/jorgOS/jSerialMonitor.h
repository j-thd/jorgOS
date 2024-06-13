#ifndef __JSERIALMONITOR_H__
#define __JSERIALMONITOR_H__

#include <stdio.h>

#ifdef JSM_ENABLE
    #define JSM_PRINTF printf
#else 
    #define JSM_PRINTF (0)
#endif // JSM_ENABLE

#include "runtime_environment.h"

void JSM_init(UART0_Type * UARTn_for_serial_monitor);
void JSM_transmit_buffer();
static void transmit_char();
static uint8_t calc_capacity();


#endif // __JSERIALMONITOR_H__