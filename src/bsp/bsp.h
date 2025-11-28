#ifndef __BSP_H__
#define __BSP_H__

#include "runtime_environment.h"
#include "jEvent.h"
#include "jEventQueueThread.h"

#define SYS_CLOCK_HZ 16000000U
#define BSP_TICKS_PER_SEC 100U

void BSP_init(void);
void BSP_tick(void);

// Switches
#define SWITCH_1 (1U << 4)
#define SWITCH_2 (1U << 0)

void BSP_SWITCH_1_init(void);
void BSP_SWITCH_2_init(void);

// BSP Events
void BSP_register_EQT_thread(OS_EventQueue_Thread *);
// BSP signals
enum BSP_Signals{
    // Start with USER_SIG which is the first allowed user signal
    BUTTON_1_DEPRESSED = USER_SIG,
    BUTTON_1_RELEASED,
    BUTTON_2_DEPRESSED,
    BUTTON_2_RELEASED
};



#endif //__BSP_H_