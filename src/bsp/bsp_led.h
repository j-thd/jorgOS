#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include <stddef.h>

#include "runtime_environment.h"
#include "bsp_gpio.h"
#include "jfp.h"

// Number the LEDs so functions can start to distinguish between LEDS that need
// to be actuated.
enum LED_INDEX {
    LED_0,
    LED_1,
    LED_TOTAL_AMOUNT // Used for checking the idx.
};

/// @brief Holds the addresses of the comparators that need to be updated for
/// one RGB led to be updated.
typedef struct LED_PWM_CMP_Addresses {
    uint32_t * red;
    uint32_t * green;
    uint32_t * blue;
} LED_PWM_CMP_Addresses;


// PWM MODULES USED FOR EACH LED
#define LED_0_RED_PWM_MODULE   PWM1
#define LED_0_GREEN_PWM_MODULE PWM1
#define LED_0_BLUE_PWM_MODULE  PWM1

#define LED_1_RED_PWM_MODULE   PWM1
#define LED_1_GREEN_PWM_MODULE PWM1
#define LED_1_BLUE_PWM_MODULE  PWM1

// COMPARATORS USED FOR EACH LED
#define LED_0_RED_CMP   _2_CMPB // PIN F1 - M1PWM5 - Generator 2b 
#define LED_0_GREEN_CMP _3_CMPB // PIN F3 - M1PWM7 - Generator 3b
#define LED_0_BLUE_CMP  _3_CMPA // PIN F2 - M1PWM6 - Generator 3a

#define LED_1_RED_CMP   _1_CMPB // PIN E5 - M1PWM3 - Generator 1b
#define LED_1_GREEN_CMP _0_CMPB // PIN D1 - M1PWM1 - Generator 0b
#define LED_1_BLUE_CMP  _1_CMPA // PIN E4 - M1PWM2 - Generator 1a

// LED to GPIO mapping
#define LED_0_RED       GPIO_F_1
#define LED_0_BLUE      GPIO_F_2
#define LED_0_GREEN     GPIO_F_3

#define LED_1_RED       GPIO_E_5
#define LED_1_GREEN     GPIO_D_1
#define LED_1_BLUE      GPIO_E_4

// LED to PWM Mapping
#define LED_0_RED_PWM       (1U << 5)
#define LED_0_GREEN_PWM     (1U << 7)
#define LED_0_BLUE_PWM      (1u << 6)

#define LED_1_RED_PWM       (1U << 3)
#define LED_1_GREEN_PWM     (1U << 1)
#define LED_1_BLUE_PWM      (1U << 2)


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
#define LED_0_RED_PMC    ( PCTL_LED_PWM << GPIO_F_1_PMC )
#define LED_0_GREEN_PMC  ( PCTL_LED_PWM << GPIO_F_3_PMC )
#define LED_0_BLUE_PMC   ( PCTL_LED_PWM << GPIO_F_2_PMC )

#define LED_1_RED_PMC    ( PCTL_LED_PWM << GPIO_E_5_PMC )
#define LED_1_GREEN_PMC  ( PCTL_LED_PWM << GPIO_D_1_PMC )
#define LED_1_BLUE_PMC   ( PCTL_LED_PWM << GPIO_E_4_PMC )

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