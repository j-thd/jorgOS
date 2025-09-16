#include <stdlib.h>

#include "bsp_led.h"
#include "jfp.h"
#include "jAssert.h"

// LED
// Store the set colors for the LED
uint8_t BSP_LED_red = 0;
uint8_t BSP_LED_green = 0;
uint8_t BSP_LED_blue = 0;
// Use this 8-bit "clock" to update the colours
uint8_t BSP_LED_clock = 0;

void BSP_LED_set_color(uint8_t red, uint8_t green, uint8_t blue){
    BSP_LED_red = red;
    BSP_LED_green = green;
    BSP_LED_blue = blue;
}




BSP_RGB_colour BSP_LED_RGB_from_HSL(SFP_10_5 hue, SFP_10_5 sat, SFP_10_5 lightness){
    // FROM https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB

    // TODO: set bounds on HSL variables (for fixed point-arithmetic)
    

    
    SFP_10_5 chroma = SFP_10_5_MULT(SFP_10_5_ONE  - abs(lightness>>1 - SFP_10_5_ONE), sat, 0 ,0 );
    // Hue dash should still be a float, so a constant of 1/60 is needed in
    // SFP_10_5.
    // I'm assuming that if I simply declare the constant once, it will be optimized
    // and not calculated every time. I should check this in the assembly at
    // some point.
    const SFP_10_5 reciprocal_of_60 = SFP_10_5_new_F(1/60.0f);
    SFP_10_5 hue_dash = SFP_10_5_MULT(
        hue,
        reciprocal_of_60,
        0,
        0
    );

    SFP_10_5 X = SFP_10_5_MULT(
        chroma,
        (SFP_10_5_ONE - abs(SFP_10_5_MOD_2(hue_dash) - SFP_10_5_ONE)),
        0,
        0
    );

    
    // Instead of doing 6 comparisons with floats, hue_dash can also be cast to
    // an int, instead.
    uint8_t hdi = SFP_10_5_TO_INT(hue_dash);
    SFP_10_5 R,G,B;

    switch(hdi){
        case 0:
            R = chroma;
            G = X;
            B = 0;

        case 1:
            R = X;
            G = chroma;
            B = 0;

        case 2:
            R = 0;
            G = chroma;
            B = X;

        case 3:
            R = 0;
            G = X;
            B = chroma;

        case 4:
            R = X;
            G = 0;
            B = chroma;

        case 5:
            R = chroma;
            G = 0;
            B = X;

        default:
            // The default case should never be reached.
            J_ERROR();

    }




    // Calculate R_1, G_1, B_1 based on hue_dash.
    // TODO: implement a function to do this with BSP_RGB_colour being used for
    // testability.
    

    // TODO: Return actual result
    BSP_RGB_colour res = {
        .rgb = {255,255,255}
    };
    return res;
}



void BSP_LED_update(void){
    // Needs better stuff here.
}

void BSP_LED_red_on(void){
    GPIOF_AHB->DATA_BITS[(LED_RED)] = LED_RED;
}

void BSP_LED_red_off(void){
    GPIOF_AHB->DATA_BITS[(LED_RED)] = 0;
}

void BSP_LED_green_on(void){
    GPIOF_AHB->DATA_BITS[(LED_GREEN)] = LED_GREEN;
}

void BSP_LED_green_off(void){
    GPIOF_AHB->DATA_BITS[(LED_GREEN)] = 0;
}

void BSP_LED_blue_on(void){
    GPIOF_AHB->DATA_BITS[(LED_BLUE)] = LED_BLUE;
}

void BSP_LED_blue_off(void){
    GPIOF_AHB->DATA_BITS[(LED_BLUE)] = 0;
}
