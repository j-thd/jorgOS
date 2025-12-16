#include "runtime_environment.h"

#include "jAssert.h"
#include "jSerialMonitor.h"

#include "bsp_timer.h"

void BSP_timer_init(){
    // TIMER0 A initilization according to Section 11.4 from datasheet.
    
    // Turn on the Clock for the WIDE timer, since we want a big prescaler.
    SYSCTL->RCGCWTIMER |= (1 << RCGCWTIMER_TIMER_0);

    // Ensure timer is disabled before changes
    WTIMER0->CTL &= ~( 1 << TIMER_n_CTL_TAEN); // Disable timer A. Not sure if B needs to be disabled, too? It is by default anyway.


    // Set 0x4 GPTMCFG, for a WIDE 32/64-bit timer to select the 32-bit timer.
    // 32-bit must be chosen if we want to use the prescaler.
    WTIMER0->CFG &= ~(0x7 << TIMER_n_CFG_CFG);
    WTIMER0->CFG |= 0x4 << TIMER_n_CFG_CFG; 
    
    // Set to periodic mode
    WTIMER0->TAMR |= (TIMER_n_TAMR_TAMR_PERIODIC_MODE << TIMER_n_TAMR_TAMR); 

    //Set timer to count DOWN
    WTIMER0->TAMR &= ~(0 << TIMER_n_TAMR_CDIR);

    // The Timer A load value triggers the time-out event when counting up, and
    // defaults to 0xFFFFFFFF
    // I am aiming for something like milli-second accuracy, I suppose I have to
    // achieve this with the prescaler. Either way, time-out occurs after
    // 4294967295 ms in that case, which is 1100+ hours or 49 days.
    
    // Never mind, the documentation tells me that prescaler acts as a timer
    // extension when counting UP and only acts as a TRUE prescaler when
    // counting down.

    // TODO: just get the systemcoreclock and check whether its frequency makes
    // sense without prescaling. Excepting 1 MHz because the PIOSC is 16 Mhz,
    // the PLL is not used (BYPASS = 1) and SYSDIV is 0xF by default (dividing
    // by clock source by 16) As far as I know the clock is touched nowhere
    // else.
    //Anyway, long story, let's get the clock and print the value in UART
    
    

    // In the order of magnitude of 1 MHz only 4294 seconds (1+ hour) fit in 32 bits,
    // which is useless of course, so we need the pre-scaler (or time extended
    // in up-counting mode.)

    // Obviously, this makes it hard to 32-bit arithmetic to get milliseconds passed,
    // if you want to run longer than 1 hour. (Is this obvious? I guess I have a
    // new lesson to learn. How to do 64 bit integers on 32-bit chipset.)

    // Just looked that up, too, although I am not sure what the proper source
    // is, but int64_t should be defined. I guess the compiler makes the right
    // instructions.
    
    // Under the assumption that 64-bit integers requires special and multiple
    // instructions to be emitted by the compiler, let's just sure the prescaler
    // for "free" to do the division. Setting the time-out to be a suitable
    // value would be nice. I could set the time-out to expire in one
    // millisecond.
    
    // So, let's get the SystemCoreClock and divide it by 1000. Rounding errors
    // don't really matter here, the clock is only precision 1-3%.

    // This had to be done by prescaling (when counting up) or daisy-chaining(up
    // or down). Opting for a down-counting timer with prescaling, probably
    // simpler to implement and uses less timers.


    SystemCoreClockUpdate();
    JSM_PRINTF("BSP: init timer. SysCoreClock Freq.: %i\n", SystemCoreClock);
    // First get the value to pre-scale too, and ensure it fits the prescaler
    // range of at most 16 bits. We are using the 32-bit WIDE timer because we
    // want to use the prescaler, which is only available in this mode.
    uint32_t prescale_Timer0_A = SystemCoreClock / 1000u;
    J_ASSERT(prescale_Timer0_A <= UINT16_MAX);

    
    // TODO: Finally set all things for the WIDE timer, now set the prescaler.
    // EVerything should be zero by default, so we can OR in the smaller 16-bit
    // value into a 32-bit register, but otherwise we would have to ensure.
    // Actually, the OR operation makes the cast unnecessary.
    WTIMER0->TAPR |= (uint16_t)prescale_Timer0_A;
    JSM_PRINTF("BSP: init timer. Timer A, Prescale.: %i\n", prescale_Timer0_A);

    // Skip the parts of the that enable interrupts, we don't need these for
    // now, we just want to read the register.

    // Ready, we can enable the timer again.
    WTIMER0->CTL |= ( 1 << TIMER_n_CTL_TAEN);
}

uint32_t BSP_get_time_millis(){
    return WTIMER0->TAV;
}