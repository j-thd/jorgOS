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




BSP_RGB_colour BSP_LED_RGB_from_HSL(SFP_11_20 hue, SFP_11_20 sat, SFP_11_20 lightness){
    // FROM https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB

    // TODO: set bounds on HSL variables (for fixed point-arithmetic)
    // TODO: turn hue into an int to capture 0-360 range, so SFP_10_5 can be
    // convert to a range that captures the reciprocal of 1/60f
    
    // Check the ranges hue: [0,360)
    J_ASSERT(hue >= 0);
    J_ASSERT((hue/360) <= SFP_11_20_ONE );

    // Saturation [0,1]
    J_ASSERT(sat >= 0);
    J_ASSERT(sat <= SFP_11_20_ONE);

    // Lightness [0,1]
    J_ASSERT(lightness >= 0);
    J_ASSERT(lightness <= SFP_11_20_ONE);


    
    JSM_transmit_buffer();
    // Some preshift for the multiplicatin is required because SFP_11_20_ONE
    // will over flow.
    SFP_11_20 chroma = SFP_11_20_MULT(
        SFP_11_20_ONE  - abs((lightness<<1) - SFP_11_20_ONE),
         sat,
         5 ,5 );
    JSM_PRINTF("Chroma: %i\n", chroma);
    J_ASSERT(chroma >=0);
    J_ASSERT(chroma <= SFP_11_20_ONE);
    // Hue dash should still be a float, so a constant of 1/60 is needed in
    // SFP_11_20.
    // I'm assuming that if I simply declare the constant once, it will be optimized
    // and not calculated every time. I should check this in the assembly at
    // some point.
    
    const SFP_11_20 reciprocal_of_60 = SFP_11_20_new_F(1/60.0f);
    SFP_11_20 hue_dash = SFP_11_20_MULT(
        hue,
        reciprocal_of_60,
        15,
        0
    );
    JSM_PRINTF("Hue_dash: %i\n", hue_dash);
    J_ASSERT(0 <=  hue_dash);
    J_ASSERT(6*SFP_11_20_ONE >= hue_dash);
    JSM_PRINTF("Y: %i\n", abs(SFP_11_20_MOD_2(hue_dash) - SFP_11_20_ONE));
    SFP_11_20 X = SFP_11_20_MULT(
        chroma,
        (SFP_11_20_ONE - abs(SFP_11_20_MOD_2(hue_dash) - SFP_11_20_ONE)),
        5,
        5
    );
    JSM_PRINTF("X: %i\n", X);
    J_ASSERT(X >=0);
    J_ASSERT(X <= SFP_11_20_ONE);
    // Calculate R_1, G_1, B_1 based on hue_dash.
    // Instead of doing 6 comparisons with floats, hue_dash can also be cast to
    // an int, instead.
    uint8_t hdi = SFP_11_20_TO_INT(hue_dash);
    JSM_PRINTF("hdi: %i\n", hdi);
    SFP_11_20 R,G,B;

    switch(hdi){
        case 0:
            R = chroma;
            G = X;
            B = 0;
            break;

        case 1:
            R = X;
            G = chroma;
            B = 0;
            break;

        case 2:
            R = 0;
            G = chroma;
            B = X;
            break;

        case 3:
            R = 0;
            G = X;
            B = chroma;
            break;

        case 4:
            R = X;
            G = 0;
            B = chroma;
            break;

        case 5:
            R = chroma;
            G = 0;
            B = X;
            break;

        default:
            // The default case should never be reached.
            J_ERROR();
            break;

    }

    SFP_11_20 m = lightness - chroma/2;
    
    // The last step from the wiki page is adding m, here we must also multiply
    // with 255 to get the desired scale [0-255]
    R = (R + m) * 255;
    G = (G + m) * 255;
    B = (B + m) * 255;

    // Finally, the actual result (R,G,B) in the range of (0,255).
    // As I went for that range, I was wondering why, other than force of habit.
    // Nothing really requires a 0-255 range on my end, except that it neatly
    // fits into a char.
    BSP_RGB_colour res = {
        .rgb = {
            SFP_11_20_TO_INT(R),
            SFP_11_20_TO_INT(G),
            SFP_11_20_TO_INT(B)
        }
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
