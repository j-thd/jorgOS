#ifndef __BSP_H__
#define __BSP_H__

#define SYS_CLOCK_HZ 16000000U
#define BSP_TICKS_PER_SEC 100U

#define LED_RED (1U << 1)
#define LED_BLUE (1U << 2)
#define LED_GREEN (1U << 3)

void BSP_init(void);

// LED
void BSP_LED_red_off(void);
void BSP_LED_red_on(void);
void BSP_LED_green_off(void);
void BSP_LED_green_on(void);
void BSP_LED_blue_off(void);
void BSP_LED_blue_on(void);

// Switches
void BSP_SW1_init(void);
void BSP_SW2_init(void);

#endif //__BSP_H_