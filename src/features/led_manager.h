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



typedef enum LM_mode {
    LM_ALWAYS_ON_MODE,
    LM_BLINKING_MODE,
    LM_HUE_SHIFTING_MODE,
    LM_NR_OF_MODES // Not a mode but for comparing if modes are valid
} LM_mode_t;

// Structs or variables to describe the state of each mode

// LM_ALWAYS_ON_MODE
// Nothing needs to be registered here.

// LM_BLINKING_MODE
typedef struct LM_Blinking_State{
    uint16_t blinking_mode_time_on; // [ms]
    uint16_t blinking_mode_time_off; // [ms]
} LM_Blinking_State;

// HUE SHIFTING MODE
// The period must be shifted into an SFP_11_20, so there is a bound on what
// works for calculation purposes. Let's make the max 10 minutes and work down
// from there.
#define HUE_SHIFTING_MODE_PERIOD_MAX  (10U * 60U * 1000U)

typedef struct  LM_Hue_Shifting_State{
    uint16_t hue_shifting_mode_period;
} LM_Hue_Shifting_State;

// Struct holding the parameters for the current mode
typedef struct LM_mode_state {
    LM_mode_t current_mode; // This determines what variant of the union is used.
    BSP_LED_HSL hsl[LM_NR_OF_MODES];
    uint32_t t_start_millis;
    // This struct holds auxiliary state variables that are mode-specific
    struct{ 
        // ALWAYS_ON requires no aux.
        LM_Blinking_State blinking;
        LM_Hue_Shifting_State hue_shifting;
    } aux;
} LM_mode_state;


void LM_init();
void LM_tick();

void LM_set_active_mode(Led_Index_t, LM_mode_t, bool);

// Update functions for specific modes
static void LM_update_blinking_mode(Led_Index_t, uint32_t);
static void LM_update_hue_shifting_mode(Led_Index_t, uint32_t);

// Update the config for a special mode
void LM_config_always_on_mode(Led_Index_t, BSP_LED_HSL *);
void LM_config_blinking_mode(Led_Index_t, BSP_LED_HSL *, uint16_t, uint16_t);
void LM_config_hue_shifting_mode(Led_Index_t, BSP_LED_HSL *, uint16_t);

//Generic setters and getters
void LM_get_HSL(Led_Index_t, LM_mode_t, BSP_LED_HSL *);
void LM_set_HSL(Led_Index_t, LM_mode_t, BSP_LED_HSL *);


#endif // __LED_MANAGER_H__