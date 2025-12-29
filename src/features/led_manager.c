#include "led_manager.h"

#include "stdbool.h"
#include "string.h"

#include "bsp_led.h"
#include "bsp_timer.h"

#include "jAssert.h"



// Initalize all modes to zero
// This sets Lightness to zero, which means that by default all RGB values are 0.
BSP_LED_HSL hsl_colour_of_mode[LM_NR_OF_MODES] = { 0 };
uint8_t active_mode = 0; // 0 Means the MODE is OFF
uint8_t millis_per_tick = 0;
uint32_t t_start_millis[LM_NR_OF_MODES] = {0}; // Reference time for start of mode

// Auxiliary colour mode variables to remember some state for each mode.

// BLINKING MODE
uint16_t blinking_mode_time_on = 1000U; // [ms]
uint16_t blinking_mode_time_off= 500U; // [ms]

// HUE SHIFTING MODE
// The period must be shifted into an SFP_11_20, so there is a bound on what
// works for calculation purposes. Let's make the max 10 minutes and work down
// from there.
#define HUE_SHIFTING_MODE_PERIOD_MAX  (10U * 60U * 1000U)
uint16_t hue_shifting_mode_period = 1000U;

/// @brief the Led Manager LM needs to be initialized, to set a default mode and
/// to have a time reference.
/// @param start_mode one of modes defined by led_manager
void LM_init(uint8_t start_mode){
    J_ASSERT(start_mode <= LM_NR_OF_MODES);

    LM_set_active_mode(start_mode, true);

}

void LM_tick(){
    
    uint32_t t_elapsed = BSP_get_time_millis() - t_start_millis[active_mode];
    switch (active_mode)
    {
    case LM_ALWAYS_ON_MODE:
        /* code */
        BSP_LED_set_color_HSL(&hsl_colour_of_mode[LM_ALWAYS_ON_MODE]);
        break;
    
    case LM_BLINKING_MODE:
        LM_update_blinking_mode(t_elapsed);
        break;
    
    case LM_HUE_SHIFTING_MODE:
        LM_update_hue_shifting_mode(t_elapsed);
        break;

    default:
        // Falling through all cases is an error.
        J_ERROR();
        break;
    }

}

void LM_set_active_mode(uint8_t mode, bool reset){
    J_ASSERT(mode <= LM_NR_OF_MODES);
    // Resetting changes the reference time so the effect starts from the beginning
    if (reset){
        t_start_millis[mode] = BSP_get_time_millis();
    }
    active_mode = mode;

    // Immediately change the state with an early tick
    LM_tick();
    
}

/// @brief Update the blinking mode based on the elapsed time
/// @param t_elapsed (t_current - t_start)
static void LM_update_blinking_mode(uint32_t t_elapsed){
    // Get the time in the current cycle, and switch the LED on if OFF time has
    // expired
    
     uint32_t time = t_elapsed % (blinking_mode_time_on + blinking_mode_time_off);
     if (time > blinking_mode_time_off){
        // Switch LED on 
        // Below the LED was switched off by setting Lightness to 0, so now we
        // just update it with the stored HSL value
        BSP_LED_set_color_HSL(&hsl_colour_of_mode[LM_BLINKING_MODE]);

     }
     else {
        // Switch LED off
        // The LED can be turned off by simply turning the lightness to 0
        // The pointer is not used later, so it can just go out of scope.
        // If I used Rust, it would of course check this for me, though :)
        BSP_LED_HSL temp_hsl = {
            hsl_colour_of_mode[LM_BLINKING_MODE][0],
            hsl_colour_of_mode[LM_BLINKING_MODE][1],
            0
        };
        BSP_LED_set_color_HSL(&temp_hsl);
     }

}

/// @brief Update the hue shifting mode based on the elapsed time
/// @param t_elapsed (t_current - t_start)
static void LM_update_hue_shifting_mode(uint32_t t_elapsed){

    // Convert the time elapsed to a new hue [0-360]
    // Since an SFP_11_20 is expected, and the current period is 10_000U we must
    // shift it in gradually to maintain any precision at all.
    // I wonder if this is the proper way to do it, but it is good to learn
    // about what problems you run into with fixed-point numbers.

    // It needs to be shifted left by 20, and by the maximum set above, it can
    // only be shifted left by 11 first before going out of bounds.
    

    SFP_11_20 new_hue = 
        ((t_elapsed % hue_shifting_mode_period << 11 ) / 
        hue_shifting_mode_period << 9) * 
        360U;


    hsl_colour_of_mode[LM_HUE_SHIFTING_MODE][0] = new_hue;
    BSP_LED_set_color_HSL(&hsl_colour_of_mode[LM_HUE_SHIFTING_MODE]);
        
}

// Mode configurations

/// @brief Set the permanent colour for the on mode or set lightness to 0 to
/// turn the LED OFF
/// @param hsl BSP_LED_HSL_TYPE [hue, saturation, lightness]
void LM_config_always_on_mode(BSP_LED_HSL * hsl){
    memcpy(hsl_colour_of_mode[LM_ALWAYS_ON_MODE], hsl, sizeof(BSP_LED_HSL));
    // Tick to immediately update
    LM_tick();
}
/// @brief Configures blinking mode to blink with HSL colour
/// @param hsl 
/// @param t_on_millis time on in milliseconds
/// @param t_off_millis time off in milliseconds
void LM_config_blinking_mode(BSP_LED_HSL * hsl, uint16_t t_on_millis, uint16_t t_off_millis){
    memcpy(hsl_colour_of_mode[LM_BLINKING_MODE], hsl, sizeof(BSP_LED_HSL));
    blinking_mode_time_on = t_on_millis;
    blinking_mode_time_off = t_off_millis;
    // Tick to immediately update
    LM_tick();
}

/// @brief Configures hue shifting mode to rotate through hue with fixed
/// saturation and lightness
/// @param hsl Hue is ignored, but sets saturation and lightness
/// @param period_millis period of rotation through hue in milliseconds
void LM_config_hue_shifting_mode(BSP_LED_HSL * hsl, uint16_t period_millis){
    memcpy(hsl_colour_of_mode[LM_HUE_SHIFTING_MODE], hsl, sizeof(BSP_LED_HSL));
    hue_shifting_mode_period = period_millis;

    // Tick to immediately update
    LM_tick();
}
