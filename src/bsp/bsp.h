#ifndef __BSP_H__
#define __BSP_H__

#include "runtime_environment.h"
#include "jEvent.h"
#include "jEventQueueThread.h"

#define SYS_CLOCK_HZ 16000000U
#define BSP_TICKS_PER_SEC 100U

void BSP_init(void);
void BSP_tick(void);

// BSP Events
void BSP_register_EQT_thread(OS_EventQueue_Thread *);
// BSP signals
enum BSP_Signals{
    // Start with USER_SIG which is the first allowed user signal
    BUTTON_1_DEPRESSED = USER_SIG,
    BUTTON_1_RELEASED,
    BUTTON_2_DEPRESSED,
    BUTTON_2_RELEASED,
    KNOB_1_CLOCKWISE,
    KNOB_1_ANTI_CLOCKWISE
};



#endif //__BSP_H_