#include <stdlib.h>

#include "bsp_led.h"

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




BSP_RGB_colour BSP_LED_RGB_from_HSL(uint16_t hue, uint16_t sat, uint16_t lightness){
    // FROM https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB

    // TODO: set bounds on HSL variables (for fixed point-arithmetic)
    #define FP_1 1000
    int16_t chroma = (FP_1  - abs(2*lightness - FP_1)) * sat; 

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
