#include "led_manager.h"

#include "stdbool.h"
#include "string.h"

#include "bsp_led.h"
#include "bsp_timer.h"

#include "jAssert.h"
#include "jUtil.h"

// The BSP exposes the amount of RGB LEDs which can be used to store the mode
// here. The code is less complex and back-and-forth state changes are more convenient if all
// states are stored when not active. This means everything must be initialized
// with sane/valid defaults. There are really no consequences for weird
// defaults, though. Only for invalid ones.

#define LM_MODE_STATE_INIT {\
    .aux={\
        .blinking={\
            .blinking_mode_time_on=2000U,\
            .blinking_mode_time_off=1000U\
        },\
        .hue_shifting={\
            .hue_shifting_mode_period=10000U\
        }\
    }\
}

LM_mode_state LM_state[LED_TOTAL_AMOUNT] = {
    LM_MODE_STATE_INIT,
    LM_MODE_STATE_INIT,
};

/// @brief the Led Manager LM needs to be initialized. With the current way LM
/// works, nothing more is needed than a tick to switch from initial LED PWM CMP
/// values to the initial modes.
void LM_init(void){
    LM_tick();
}

void LM_tick(){
    
    // Iterate through each LED and its mode
    for (int LED_idx=0; LED_idx < LED_TOTAL_AMOUNT; LED_idx++ ){

        uint32_t t_elapsed = BSP_get_time_millis() - LM_state[LED_idx].t_start_millis;
        switch (LM_state[LED_idx].current_mode)
            {
            case LM_ALWAYS_ON_MODE:
                /* code */
                BSP_LED_set_color_HSL(LED_idx, &LM_state[LED_idx].hsl[LM_ALWAYS_ON_MODE]);
                break;
            
            case LM_BLINKING_MODE:
                LM_update_blinking_mode(LED_idx, t_elapsed);
                break;
            
            case LM_HUE_SHIFTING_MODE:
                LM_update_hue_shifting_mode(LED_idx, t_elapsed);
                break;

            default:
                // Falling through all cases is an error.
                J_ERROR();
                break;
            }

    }


}

void LM_set_active_mode(Led_Index_t LED_idx, LM_mode_t mode, bool reset){
    J_ASSERT(mode <= LM_NR_OF_MODES);
    // Resetting changes the reference time so the effect starts from the beginning
    if (reset){
        LM_state[LED_idx].t_start_millis  = BSP_get_time_millis();
    }
    LM_state[LED_idx].current_mode = mode;

    // Immediately change the state with an early tick
    LM_tick();
    
}

/// @brief Update the blinking mode based on the elapsed time
/// @param t_elapsed (t_current - t_start)
static void LM_update_blinking_mode(Led_Index_t LED_idx, uint32_t t_elapsed){
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    // Get the time in the current cycle, and switch the LED on if OFF time has
    // expired
    
    uint32_t time = t_elapsed % 
        ( LM_state[LED_idx].aux.blinking.blinking_mode_time_on +
             LM_state[LED_idx].aux.blinking.blinking_mode_time_off);
    if (time > LM_state[LED_idx].aux.blinking.blinking_mode_time_off){
        // Switch LED on 
        // Below the LED was switched off by setting Lightness to 0, so now we
        // just update it with the stored HSL value
        BSP_LED_set_color_HSL(LED_idx, &LM_state[LED_idx].hsl[LM_BLINKING_MODE]);

    }
    else {
        // Switch LED off
        // The LED can be turned off by simply turning the lightness to 0, the other
        // variables being arbitrary in value.
        // The pointer is not used later, so it can just go out of scope.
        // If I used Rust, it would of course check this for me, though :)
        BSP_LED_HSL temp_hsl = { 0 };
        BSP_LED_set_color_HSL(LED_idx, &temp_hsl);
    }

}

/// @brief Update the hue shifting mode based on the elapsed time
/// @param t_elapsed (t_current - t_start)
static void LM_update_hue_shifting_mode(Led_Index_t LED_idx, uint32_t t_elapsed){

    // Convert the time elapsed to a new hue [0-360]
    // Since an SFP_11_20 is expected, and the current period is 10_000U we must
    // shift it in gradually to maintain any precision at all.
    // I wonder if this is the proper way to do it, but it is good to learn
    // about what problems you run into with fixed-point numbers.

    // It needs to be shifted left by 20, and by the maximum set above, it can
    // only be shifted left by 11 first before going out of bounds.
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    
    SFP_11_20 new_hue = 
        ((t_elapsed 
            % LM_state[LED_idx].aux.hue_shifting.hue_shifting_mode_period << 11 ) / 
        LM_state[LED_idx].aux.hue_shifting.hue_shifting_mode_period << 9) * 
        360U;

    
    LM_state[LED_idx].hsl[LM_HUE_SHIFTING_MODE][0] = new_hue;
    BSP_LED_set_color_HSL(LED_idx, &LM_state[LED_idx].hsl[LM_HUE_SHIFTING_MODE]);
        
}

// Mode configurations

/// @brief Set the permanent colour for the on mode or set lightness to 0 to
/// turn the LED OFF
/// @param hsl BSP_LED_HSL_TYPE [hue, saturation, lightness]
void LM_config_always_on_mode(Led_Index_t LED_idx, BSP_LED_HSL * hsl){
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    memcpy(LM_state[LED_idx].hsl[LM_ALWAYS_ON_MODE], hsl, sizeof(BSP_LED_HSL));
    // Tick to immediately update
    LM_tick();
}
/// @brief Configures blinking mode to blink with HSL colour
/// @param hsl 
/// @param t_on_millis time on in milliseconds
/// @param t_off_millis time off in milliseconds
void LM_config_blinking_mode(Led_Index_t LED_idx, BSP_LED_HSL * hsl, uint16_t t_on_millis, uint16_t t_off_millis){
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    memcpy(LM_state[LED_idx].hsl[LM_BLINKING_MODE], hsl, sizeof(BSP_LED_HSL));
    LM_state[LED_idx].aux.blinking.blinking_mode_time_on = t_on_millis;
    LM_state[LED_idx].aux.blinking.blinking_mode_time_off = t_off_millis;
    // Tick to immediately update
    LM_tick();
}

/// @brief Configures hue shifting mode to rotate through hue with fixed
/// saturation and lightness
/// @param hsl Hue is ignored, but sets saturation and lightness
/// @param period_millis period of rotation through hue in milliseconds
void LM_config_hue_shifting_mode(Led_Index_t LED_idx, BSP_LED_HSL * hsl, uint16_t period_millis){
    memcpy(LM_state[LED_idx].hsl[LM_HUE_SHIFTING_MODE], hsl, sizeof(BSP_LED_HSL));
    LM_state[LED_idx].aux.hue_shifting.hue_shifting_mode_period = period_millis;

    // Tick to immediately update
    LM_tick();
}

/// @brief Copy hsl of the current mode out.
/// @param mode 
/// @return BSP_LED_HSL of chosen mode.
void LM_get_HSL(Led_Index_t LED_idx, LM_mode_t mode, BSP_LED_HSL * hsl){
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    // TODO: start using memcpy_s instead for safety.
    memcpy( hsl, LM_state[LED_idx].hsl[mode], sizeof(BSP_LED_HSL));

}

void LM_set_HSL(Led_Index_t LED_idx, LM_mode_t mode, BSP_LED_HSL * hsl){
    J_ASSERT(LED_idx < LED_TOTAL_AMOUNT);
    memcpy(LM_state[LED_idx].hsl[mode], hsl, sizeof(BSP_LED_HSL));    
    // Tick to immediately update
    LM_tick();
    
}
