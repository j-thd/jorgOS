/*
    A small manager that allows switching between different colour modes.
    Modes are just different stored colours, and one of the modes is given
    priority.
    
    This allows for continuously updating a colour, even when it is not being
    shown. A more important colour "message" can override the less important
    colour mode. Additionally, this also means we can potentially stash away the
    updating the colours, when using fancy effects, in some low priority thread,
    such as the idle thread.

    With this I can implement a nice not too bright colour hue-shifting loop,
    which occassionally gets overriden to display a bright red / blue or green
    colour.
    
    The logic of the fancy effects or simply setting known colours such as RED,
    BLUE, GREEN, etc can also be retained here.
*/
#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "stdbool.h"

#include "bsp_led.h"


void LM_init(uint8_t);
void LM_tick();

#define LM_NR_OF_MODES 3U

#define LM_ALWAYS_ON_MODE 0U
#define LM_BLINKING_MODE 1U
#define LM_HUE_SHIFTING_MODE 2U

void LM_set_active_mode(uint8_t, bool);

// Update functions for specific modes
static void LM_update_blinking_mode(uint32_t);
static void LM_update_hue_shifting_mode(uint32_t);

// Update the config for a special mode
void LM_config_always_on_mode(BSP_LED_HSL *);
void LM_config_blinking_mode(BSP_LED_HSL *, uint16_t, uint16_t);
void LM_config_hue_shifting_mode(BSP_LED_HSL *, uint16_t);


#endif // __LED_MANAGER_H__