#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "runtime_environment.h"

#include "jfp.h"

// LED
#define LED_RED (1U << 1)
#define LED_BLUE (1U << 2)
#define LED_GREEN (1U << 3)


// Type definitions allowing for some clarity and consitency
typedef uint8_t BSP_LED_RGB_TYPE;
typedef BSP_LED_RGB_TYPE BSP_LED_RGB[3];


typedef SFP_11_20 BSP_LED_HSL_TYPE;
typedef BSP_LED_HSL_TYPE BSP_LED_HSL[3];


void BSP_LED_red_off(void);
void BSP_LED_red_on(void);
void BSP_LED_green_off(void);
void BSP_LED_green_on(void);
void BSP_LED_blue_off(void);
void BSP_LED_blue_on(void);

void BSP_LED_set_color_RGB(BSP_LED_RGB*);
void BSP_LED_set_color_HSL(BSP_LED_HSL*);
static void BSP_LED_set_PWM_signal(BSP_LED_RGB*);


void BSP_LED_RGB_from_HSL(BSP_LED_RGB*, BSP_LED_HSL*);

// LED colours with PWM
void BSP_LED_PWM_init(void);
// The GPIOPCTL value to set the led pins to the PWM signals
#define PCTL_LED_PWM (0x5)
// Port Mux Control values to set LEDS to PWM
#define LED_RED_PMC    ( PCTL_LED_PWM << 4  )
#define LED_GREEN_PMC  ( PCTL_LED_PWM << 8  )
#define LED_BLUE_PMC   ( PCTL_LED_PWM << 12 )

#define BSP_LED_MAX_LOAD_VALUE 0xFF


// SYSCTL->RCC bitfields
#define RCC_USEPWMDIV (20)
#define RCC_PWMDIV    (17)

// PWMnGENx fields
// Possible values in bitfields
#define DRIVE_PWM_x_LOW     (0x2)
#define DRIVE_PWM_x_HIGH    (0x3)
// ACTion on LOAD value is to drive pwm x (A/B) high

#define ACTLOAD_PWM_x_HIGH (DRIVE_PWM_x_HIGH << 2) 
#define ACTCMPAD_PWM_x_LOW (DRIVE_PWM_x_LOW << 6)
// ACT on hitting CoMParator B value when counting Down is to drive pwm x (A/B)
// low 
#define ACTCMPBD_PWM_x_LOW (DRIVE_PWM_x_LOW << 10) 



#endif //__BSP_LED_H_