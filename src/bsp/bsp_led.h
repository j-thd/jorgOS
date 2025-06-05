#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "runtime_environment.h"

// LED
#define LED_RED (1U << 1)
#define LED_BLUE (1U << 2)
#define LED_GREEN (1U << 3)

void BSP_LED_red_off(void);
void BSP_LED_red_on(void);
void BSP_LED_green_off(void);
void BSP_LED_green_on(void);
void BSP_LED_blue_off(void);
void BSP_LED_blue_on(void);

void BSP_LED_set_color(uint8_t, uint8_t, uint8_t);
void BSP_LED_update(void);

// HSL to RGB utilities
typedef struct BSP_RGB_colour{
    uint8_t rgb[3];
} BSP_RGB_colour;

BSP_RGB_colour BSP_LED_RGB_from_HSL(uint16_t hue, uint16_t sat, uint16_t lightness);

// LED colours with PWM
void BSP_LED_PWM_init(void);
// The GPIOPCTL value to set the led pins to the PWM signals
#define PCTL_LED_PWM (0x5)
// Port Mux Control values to set LEDS to PWM
#define LED_RED_PMC    ( PCTL_LED_PWM << 4  )
#define LED_GREEN_PMC  ( PCTL_LED_PWM << 8  )
#define LED_BLUE_PMC   ( PCTL_LED_PWM << 12 )

#endif //__BSP_LED_H_