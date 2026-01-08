#include "runtime_environment.h"

#include "qei.h"

#include "jSerialMonitor.h"

/// @brief Initializes QEI module 0, which uses pins:
/// * PD6: Phase A 0
/// * PD7: Phase B 0
/// * PD3: Index signal is unused, so free to use elsewhere
void QEI_0_init(void){
    // Based on example on Section 21.4 of Datasheet(page 1310)
    // Enable the clock
    SYSCTL->RCGCQEI |= 1U << RCGCQEI_R0;

    // Enable GPIO D clock.
    SYSCTL->RCGCGPIO |= 1U <<  RCGCGPIO_D;

    //GPIO AHB is not enabled yet
    SYSCTL->GPIOHBCTL |= 1U << SYSCTL_GPIOHBCTL_GPIO_D;

    // GPIO Digital Enable must also be set , as well as pull-down resistors and
    // PD7 is locked, so we need to unlock it.
    GPIOD_AHB->LOCK = 0x4C4F434B;
    GPIOD_AHB->CR |= 1U << 7;

    GPIOD_AHB->DEN |= (1U << 3) | (1U << 6) | (1U << 7);
    GPIOD_AHB->DIR &= (1U << 3) | (1U << 6) | (1U << 7);
    GPIOD_AHB->PDR |= (1U << 3) | (1U << 6) | (1U << 7);

    // Set the pins to the alternate function for pin 3, 6 & 7.
    GPIOD_AHB->AFSEL |=  (1U << 3) | (1U << 6) | (1U << 7);
    GPIOD_AHB->CR &= ~(1U << 7);
    GPIOD_AHB->LOCK = 0x0;

    // This is a bit more complicated bit operation, as before I dealt with
    // multiple bits when all had to be set to 1, and none had to be or remain
    // zero.
    
    // So, I think this is the process. First I zero the bits, by doing an AND
    // with the bitmask.
    // Then I OR with the actual value I want there. 
    // I can do this for the bits of all pins combined in one step by combining
    // all masks for the zero-ing, and then just OR-ing all others in one-by-one
    // I should at some point check if this is the right way to do it.
    GPIOD_AHB->PCTL = 
        (GPIOD_AHB->PCTL & 
            ~(GPIO_PMC_PIN_3_MASK | GPIO_PMC_PIN_6_MASK | GPIO_PMC_PIN_7_MASK)
            ) | 
        (GPIO_PMC_VAL_FOR_QEI << GPIO_PMC_PIN_3) |
        (GPIO_PMC_VAL_FOR_QEI << GPIO_PMC_PIN_6) |
        (GPIO_PMC_VAL_FOR_QEI << GPIO_PMC_PIN_7);
    
    // Unlike the example, we have no index pulse, so no need to reset when
    // capturing it. The filter is enabled so we can debounce the switching
    // mechanism.
    QEI0->CTL |=  (1U << QEI_CTL_FILTEN) | 
        (0xF << QEI_CTL_FILTCNT);
    QEI0->MAXPOS = 0xFFFFFFFF; // There is no special value we need in this case, we just want to turn a knob left and right and register it.
    
    QEI0->CTL |= 1U << QEI_CTL_ENABLE;
    QEI0->POS = 0x8000000U;
    JSM_PRINTF("[QEI] Initialized... Pos: %u\n",QEI0->POS);
}

uint32_t QEI_0_get_position(void){
    return QEI0->POS;
}
