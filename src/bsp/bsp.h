#ifndef __BSP_H__
#define __BSP_H__

#define SYS_CLOCK_HZ 16000000U
#define BSP_TICKS_PER_SEC 100U

// Currently, each button is manually entered in a debounce procedure of bsp.c
// and manually initialized in BSP_init()
// The maximum is just used to have the appropriate size for call-back functions.
// #define MAX_REGISTERED_BUTTONS 2
// typedef void (*BSP_Button_Depressed_Handler);
// typedef void (*BSP_Button_Released_Handler);
// void BSP_register_button( uint8_t,
//     BSP_Button_Depressed_Handler *, BSP_Button_Released_Handler * );

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

// Switches
#define SWITCH_1 (1U << 4)
#define SWITCH_2 (1U << 0)

void BSP_SWITCH_1_init(void);
void BSP_SWITCH_2_init(void);

#endif //__BSP_H_