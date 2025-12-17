/*
    BSP timer: functions to get elapsed time and similar from registers.
    Clocks on device are not expected to be super-precise. There is a crystal on
    the board, but it is only hooked up to the hibernate module.
*/
#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include <stdint.h>

// Register bit/field constants

#define RCGCWTIMER_TIMER_0 0

#define TIMER_n_CTL_TAEN 0
#define TIMER_n_CFG_CFG 0
#define TIMER_n_TAMR_TAMR 0
#define TIMER_n_TAMR_CDIR 4


// Register bit/field values
#define TIMER_n_TAMR_TAMR_PERIODIC_MODE 0x2


void BSP_timer_init();
uint32_t BSP_get_time_millis();

// Calculation constants
#define WTIMER_0_A_STARTING_COUNT 0xFFFFFFFF // Reset value in Interval Load register before counting down


#endif // __BSP_TIMER_H