#include "bsp_switch.h"
#include "bsp.h"

#include "runtime_environment.h"

#include "jAssert.h"
#include "jEvent.h"
#include "jEventQueueThread.h"

void BSP_SWITCH_1_init(void){
    // Set the switch as an input (0), enable digital
    GPIOF_AHB->DIR &= ~SWITCH_1;
    GPIOF_AHB->DEN |= SWITCH_1;

    // Pull-up resistor
    GPIOF_AHB->PUR |= SWITCH_1;


}
void BSP_SWITCH_2_init(void){
    // The registers for Switch 2 are protected, and must be unlocked first.
    GPIOF_AHB->LOCK = 0x4C4F434B;
    GPIOF_AHB->CR |= SWITCH_2;
    // Set the switch as an input (0), enable digital
    GPIOF_AHB->DIR &= ~SWITCH_2;
    GPIOF_AHB->DEN |= SWITCH_2;

    // Pull-up resistor
    GPIOF_AHB->PUR |= SWITCH_2;

    // Restore and relock the commit register
    GPIOF_AHB->CR &= ~SWITCH_2;
    GPIOF_AHB->LOCK = 0U;

}

/// @brief Debounces the buttons and posts resulting events to the big
/// overarching EventQueueThread(EQT) for BSP events.
/// @param BSP_EQT_thread must be a reference to the big shared EQT thread.
void BSP_switch_tick(OS_EventQueue_Thread * BSP_EQT_thread){
    J_REQUIRE_IN_CRIT_SEC;
    static struct DebouncedButtons {
        uint32_t depressed;
        uint32_t previous;
    } buttons = {0U, 0U};

    

    uint32_t current;
    uint32_t temp;

    current = ~GPIOF_AHB->DATA_BITS[SWITCH_1 | SWITCH_2];
    temp = buttons.depressed;
    buttons.depressed |= (buttons.previous & current); // Set depressed buttons. (down two ticks in a row)
    buttons.depressed &= (buttons.previous | current); // Set released buttons (up two ticks in a row)
    buttons.previous = current;

    // Now check if the depressed state changed.
    temp ^= buttons.depressed;

    if ( temp & SWITCH_1 ){ // Switch 1 state changed
        if (buttons.depressed & SWITCH_1){ // Switch 1 was pressed this tick
            J_Event e = {BUTTON_1_DEPRESSED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 1 pressed\n");
        }
        else { // The switch was just released
            J_Event e = {BUTTON_1_RELEASED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 1 released\n");
        }
    }

    if ( temp & SWITCH_2 ){ // Switch 1 state changed
        if (buttons.depressed & SWITCH_2){ // Switch 2 was pressed this tick
            J_Event e = {BUTTON_2_DEPRESSED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 2 pressed\n");
        }
        else { // The switch was just released
            J_Event e = {BUTTON_2_RELEASED};
            OS_EQT_post(BSP_EQT_thread, e);
            //JSM_PRINTF("Switch 2 released\n");
        }
    }

}