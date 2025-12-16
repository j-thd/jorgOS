#include "led_manager.h"

#include "bsp_led.h"

#include "jAssert.h"

#define NR_OF_MODES 2U


// Initalize all modes to zero
// This sets Lightness to zero, which means that by default all RGB values are 0.
BSP_LED_HSL hsl_colour_of_mode[NR_OF_MODES] = { 0 };
uint8_t active_mode = 0; // 0 Means the MODE is OFF
uint8_t millis_per_tick = 0;

/// @brief the Led Manager LM cannot function properly on some fancy effects if
/// it does not know how long a tick is. We must also find a source of a stable
/// tick. A simply thread is fine, imo.
/// @param milliseconds_per_tick 
void LM_init(uint8_t milliseconds_per_tick){
    J_ASSERT(milliseconds_per_tick > 0);
}

void LM_set_active_mode(uint8_t);