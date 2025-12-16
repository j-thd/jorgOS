#include <stdlib.h>

#include "runtime_environment.h"
#include "bsp_led.h"
#include "jfp.h"
#include "jAssert.h"

/// @brief Set Colour using HSL
/// @param hue 0-360
/// @param saturation 0-1
/// @param lightness 0-1
void BSP_LED_set_color_HSL(BSP_LED_HSL* hsl){
    BSP_LED_RGB rgb; 
    BSP_LED_RGB_from_HSL(&rgb, hsl);
    BSP_LED_set_PWM_signal(&rgb);

}

void BSP_LED_set_color_RGB(BSP_LED_RGB * p_rgb){
    BSP_LED_set_PWM_signal(p_rgb);
}


/// @brief Take the RGB value and convert it to a suitable CMP value in the PWM
/// registers. This is where we can tweak the non-linear behaviour of the
/// LED/PWM combo too. Also, if the value is 0, the PWM must be turned off
/// completely, as a faint light will always be visible otherwise. Function
/// @param red 0-255 value
/// @param green 0-255 value
/// @param blue 0-255 value
static void BSP_LED_set_PWM_signal(BSP_LED_RGB * p_rgb){
    // Multiply first with MAX load value for less rounding errors
    // MAX LOAD value was 10k at time of writing code, so 16-bit is enough

    // The signal is inverted so CMP values actually drive the target high now.
    // The inversion stops the tiny blip of light when you went the LED to be dark.

    // The "order" of the PWM signals is rBG instead or rGB.
    PWM1->_2_CMPB = (((BSP_LED_RGB_TYPE *)p_rgb)[0] * (BSP_LED_MAX_LOAD_VALUE - 1) ) / 256; // This value drives 2pwmB low // RED
    PWM1->_3_CMPA = (((BSP_LED_RGB_TYPE *)p_rgb)[2] * (BSP_LED_MAX_LOAD_VALUE - 1) ) / 256;// This value drives 3pwmA low // BLUE
    PWM1->_3_CMPB = (((BSP_LED_RGB_TYPE *)p_rgb)[1] * (BSP_LED_MAX_LOAD_VALUE - 1) ) / 256;// This value drives 3pwmB low // GREEN

    JSM_PRINTF("\nR: %i\tG: %i\tB: %i\n", 
        ((BSP_LED_RGB_TYPE *)p_rgb)[0], 
        ((BSP_LED_RGB_TYPE *)p_rgb)[1], 
        ((BSP_LED_RGB_TYPE *)p_rgb)[2]
    );

}


/// @brief Take Hue, Saturation and Lightness value and return them in p_rgb.
/// @param p_rgb 
/// @param hue 
/// @param sat 
/// @param lightness 
void BSP_LED_RGB_from_HSL(BSP_LED_RGB * p_rgb, BSP_LED_HSL* p_hsl){
    // FROM https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
    
    // Check the ranges hue: [0,360]
    J_ASSERT(((BSP_LED_HSL_TYPE *)p_hsl)[0] >= 0);
    J_ASSERT((((BSP_LED_HSL_TYPE *)p_hsl)[0]/360) <= SFP_11_20_ONE );

    // Saturation [0,1]
    J_ASSERT(((BSP_LED_HSL_TYPE *)p_hsl)[1] >= 0);
    J_ASSERT(((BSP_LED_HSL_TYPE *)p_hsl)[1] <= SFP_11_20_ONE);

    // Lightness [0,1]
    J_ASSERT(((BSP_LED_HSL_TYPE *)p_hsl)[2] >= 0);
    J_ASSERT(((BSP_LED_HSL_TYPE *)p_hsl)[2] <= SFP_11_20_ONE);


    
    //JSM_transmit_buffer();
    // Some preshift for the multiplicatin is required because SFP_11_20_ONE
    // will overflow without it.
    SFP_11_20 chroma = SFP_11_20_MULT(
        SFP_11_20_ONE  - abs((((BSP_LED_HSL_TYPE *)p_hsl)[2]<<1) - SFP_11_20_ONE),
         ((BSP_LED_HSL_TYPE *)p_hsl)[1],
         5 ,5 );

    J_ASSERT(chroma >=0);
    J_ASSERT(chroma <= SFP_11_20_ONE);
    // Hue dash should still be a float, so a constant of 1/60 is needed in
    // SFP_11_20.
    // I'm assuming that if I simply declare the constant once, it will be optimized
    // and not calculated every time. I should check this in the assembly at
    // some point.
    
    const SFP_11_20 reciprocal_of_60 = SFP_11_20_new_F(1/60.0f);
    SFP_11_20 hue_dash = SFP_11_20_MULT(
        ((BSP_LED_HSL_TYPE *)p_hsl)[0],
        reciprocal_of_60,
        15,
        0
    );
    // JSM_PRINTF("Hue_dash: %i\n", hue_dash);
    J_ASSERT(0 <=  hue_dash);
    J_ASSERT(6*SFP_11_20_ONE >= hue_dash);
    //JSM_PRINTF("Y: %i\n", abs(SFP_11_20_MOD_2(hue_dash) - SFP_11_20_ONE));
    SFP_11_20 X = SFP_11_20_MULT(
        chroma,
        (SFP_11_20_ONE - abs(SFP_11_20_MOD_2(hue_dash) - SFP_11_20_ONE)),
        5,
        5
    );
    // JSM_PRINTF("X: %i\n", X);
    J_ASSERT(X >=0);
    J_ASSERT(X <= SFP_11_20_ONE);
    // Calculate R_1, G_1, B_1 based on hue_dash.
    // Instead of doing 6 comparisons with floats, hue_dash can also be cast to
    // an int, instead.
    uint8_t hdi = SFP_11_20_TO_INT(hue_dash);
    // JSM_PRINTF("hdi: %i\n", hdi);
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

    SFP_11_20 m = ((BSP_LED_HSL_TYPE *)p_hsl)[2] - chroma/2;
    
    // The last step from the wiki page is adding m, here we must also multiply
    // with 255 to get the desired scale [0-255]
    
    // Finally, the actual result (R,G,B) in the range of (0,255).
    // As I went for that range, I was wondering why, other than force of habit.
    // Nothing really requires a 0-255 range on my end, except that it neatly
    // fits into a char.

    // So, when using a typedeffed array, you cannot simply dereference, as you
    // will dereference to that array type. I'm not sure what the clean
    // idiomatic solution is for keeping some values together in C.

    ((BSP_LED_RGB_TYPE*)p_rgb)[0] = SFP_11_20_TO_INT((R + m) * 255);
    ((BSP_LED_RGB_TYPE*)p_rgb)[1] = SFP_11_20_TO_INT((G + m) * 255);
    ((BSP_LED_RGB_TYPE*)p_rgb)[2] = SFP_11_20_TO_INT((B + m) * 255);
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

// LED COLOURS THROUGH PWM
void BSP_LED_PWM_init(void){
    // This is partially based on the TM4C datasheet page 1239. Register names
    // seem to be deprecated in this manual.
    SYSCTL->RCGCPWM |= (1U << 1); // Enable clock for PWM1
    // Set the pins to alternative functions so it can be set respond to PWM
    // signals
    GPIOF_AHB->AFSEL |= (LED_RED | LED_GREEN | LED_BLUE);
    //Set the Port Control to the PWM signals.
    GPIOF_AHB->PCTL |= (LED_RED_PMC | LED_GREEN_PMC | LED_BLUE_PMC);
    // Turn on the clock divisor for PWM
    SYSCTL->RCC |= 1U << RCC_USEPWMDIV;
    // Set the divisor to 2 with 0x0, realizing it is 0x7 by default.
    // So I think it must be cleared first with 3 1-bits (0x7), if you want to
    // set anything but 0x0.
    SYSCTL->RCC &= ~(0x7 << RCC_PWMDIV);
    
    // This just puts the control for block PWM1 Block 2 to the default
    // settings, which mostly consists bitfields for it being enabled and how it
    // should deal with updates to particular values (synchronization). Also,
    // something about fault handling.
    // PWM Block 1 Generator 2 b is for Pin F1 (red LED)
    PWM1->_2_CTL = 0x0;
    // PWM Block 1 Generator 3 a & b is for Pin F2 & F3 (blue & green LED)
    PWM1->_3_CTL = 0x0;
    
    // RED LED (Generator 2 pwm b)
    // Drive the pwmB signal high on LOAD value and low on CMP B value (when
    // counting down)
    PWM1->_2_GENB = ( ACTLOAD_PWM_x_HIGH | ACTCMPBD_PWM_x_LOW );
    // Blue LED (Generator 3 pwm A)
    // Drive the pwmA signal high on LOAD value and low on CMP A value (when
    // counting down)
    PWM1->_3_GENA = ( ACTLOAD_PWM_x_HIGH | ACTCMPAD_PWM_x_LOW );
    // Green LED
    // Drive the pwmB signal high on LOAD value and low on CMP B value (when
    // counting down)
    PWM1->_3_GENB = ( ACTLOAD_PWM_x_HIGH | ACTCMPBD_PWM_x_LOW );

    // Provided that the clock is 10 MHz (System clock divided by 2 with PWMDIV
    // 399 for 400 ticks per period for 40 microseconds resulting in 25 kHz
    PWM1->_2_LOAD = BSP_LED_MAX_LOAD_VALUE; // This value drives pwm high according to GENx settings
    PWM1->_3_LOAD = BSP_LED_MAX_LOAD_VALUE;
    // 99 for 75% duty cycle (100/400). 

    // Inverting the signal might get rid of the tiny blip when the cmp value is
    // 0 (although I suppose it introduces a tiny dimming too, but don't really
    // care about that. That won't be visible at all.)
    PWM1->INVERT = 0x7 << 5;

    PWM1->_2_CTL = 0x1; // Enable PWM 1 Generator 2
    PWM1->_3_CTL = 0x1; // Enable PWM 1 Generator 3
    PWM1->ENABLE = 0x7 << 5; // Enable pwm5(3b), 6(4a) and 7(4b)
}