#include "jEventQueueThread.h"

#define SWITCH_1 (1U << 4)
#define SWITCH_2 (1U << 0)

void BSP_SWITCH_1_init(void);
void BSP_SWITCH_2_init(void);

void BSP_switch_tick(OS_EventQueue_Thread *); // Update triggered switches.