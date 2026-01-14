#ifndef __QEI_H__
#define __QEI_H__

#include <stdint.h>
#include "jEventQueueThread.h"

// Defined constants
#define RCGCQEI_R0 0U
#define RCGCGPIO_D 3U
#define GPIO_PMC_VAL_FOR_QEI 6U // The value to set the alternate function to QEI in GPIO Port Control
#define SYSCTL_GPIOHBCTL_GPIO_D 3U

// Pins
#define GPIO_PMC_PIN_3 (3U*4) // Got to to do some proper bit-masking here...
#define GPIO_PMC_PIN_6 (6U*4)
#define GPIO_PMC_PIN_7 (7U*4)

// Masks for pins
#define GPIO_PMC_PIN_3_MASK (0xFU << GPIO_PMC_PIN_3) // Creates a mask for the 4 bits that can be set
#define GPIO_PMC_PIN_6_MASK (0xFU << GPIO_PMC_PIN_6)
#define GPIO_PMC_PIN_7_MASK (0xFU << GPIO_PMC_PIN_7) // Creates a mask for the 4 bits that can be set

// QEI CTL Fields
#define QEI_CTL_ENABLE 0U
#define QEI_CTL_CAPMODE 3U
#define QEI_CTL_FILTEN 13U
#define QEI_CTL_FILTCNT 16U


void QEI_0_init(void);
uint32_t QEI_0_get_position(void);
void QEI_tick(OS_EventQueue_Thread *);


#endif // __QEI_H__