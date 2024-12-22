#ifndef __BSP_H__
#define __BSP_H__

#include "runtime_environment.h"
#include "jEvent.h"
#include "jEventQueueThread.h"

#define SYS_CLOCK_HZ 16000000U
#define BSP_TICKS_PER_SEC 100U

void BSP_init(void);
void BSP_tick(void);

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

// LED colours with PWM
void BSP_LED_PWM_init(void);
// The GPIOPCTL value to set the led pins to the PWM signals
#define PCTL_LED_PWM (0x5)
// Port Mux Control values to set LEDS to PWM
#define LED_RED_PMC    ( PCTL_LED_PWM << 4  )
#define LED_GREEN_PMC  ( PCTL_LED_PWM << 8  )
#define LED_BLUE_PMC   ( PCTL_LED_PWM << 12 )


// Switches
#define SWITCH_1 (1U << 4)
#define SWITCH_2 (1U << 0)

void BSP_SWITCH_1_init(void);
void BSP_SWITCH_2_init(void);

// BSP Events
void BSP_register_EQT_thread(OS_EventQueue_Thread *);
// BSP signals
enum BSP_Signals{
    // Start with USER_SIG which is the first allowed user signal
    BUTTON_1_DEPRESSED = USER_SIG,
    BUTTON_1_RELEASED,
    BUTTON_2_DEPRESSED,
    BUTTON_2_RELEASED
};

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

#endif //__BSP_H_