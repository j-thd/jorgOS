// First an attempt was made to also make a startup file in GAS, but the documentation for ARM was pretty much non-existent.
//

#include <stdint.h>

#include <system_TM4C123.h>

// Vector table must go here





// Declare a weak Reset Handler that can be overridden elsewhere
extern void _start(void); // This should be defined in crt0.S
void Reset_Handler(void) __attribute__((weak));

// Refer to the dummy handler
void Default_Handler(void);

extern void NMI_Handler                (void) __attribute__((weak, alias("Default_Handler")));
extern void HardFault_Handler          (void) __attribute__((weak, alias("Default_Handler")));
extern void MemManage_Handler          (void) __attribute__((weak, alias("Default_Handler")));
extern void BusFault_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void UsageFault_Handler         (void) __attribute__((weak, alias("Default_Handler")));
extern void SVC_Handler                (void) __attribute__((weak, alias("Default_Handler")));
extern void DebugMon_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void PendSV_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void SysTick_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOA_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOB_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOC_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOD_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOE_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART0_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART1_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void SSI0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void PMW0_FAULT_Handler         (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM0_0_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM0_1_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM0_2_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void QEI0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC0SS0_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC0SS1_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC0SS2_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC0SS3_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void WDT0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER0A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER0B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER1A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER1B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER2A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER2B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void COMP0_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void COMP1_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void COMP2_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void SYSCTL_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void FLASH_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOF_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOG_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOH_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART2_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void SSI1_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER3A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER3B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C1_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void QEI1_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void CAN0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void CAN1_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void CAN2_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void HIB_Handler                (void) __attribute__((weak, alias("Default_Handler")));
extern void USB0_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM0_3_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void UDMA_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void UDMAERR_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC1SS0_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC1SS1_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC1SS2_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void ADC1SS3_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOJ_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOK_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOL_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void SSI2_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void SSI3_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void UART3_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART4_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART5_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART6_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void UART7_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C2_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C3_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER4A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER4B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER5A_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void TIMER5B_Handler            (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER0A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER0B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER1A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER1B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER2A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER2B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER3A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER3B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER4A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER4B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER5A_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void WTIMER5B_Handler           (void) __attribute__((weak, alias("Default_Handler")));
extern void FPU_Handler                (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C4_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void I2C5_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOM_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPION_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void QEI2_Handler               (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP0_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP1_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP2_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP3_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP4_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP5_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP6_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOP7_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ0_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ1_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ2_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ3_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ4_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ5_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ6_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOQ7_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOR_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void GPIOS_Handler              (void) __attribute__((weak, alias("Default_Handler")));
extern void PMW1_0_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM1_1_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM1_2_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM1_3_Handler             (void) __attribute__((weak, alias("Default_Handler")));
extern void PWM1_FAULT_Handler         (void) __attribute__((weak, alias("Default_Handler")));


extern uint32_t __stack_start__; // Defined in the linker script, in a presized-section.

uint32_t vector_table[] __attribute__((section(".vector_table"))) = {
    (uint32_t)&__stack_start__,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMon_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
    (uint32_t)&GPIOA_Handler,
    (uint32_t)&GPIOB_Handler,
    (uint32_t)&GPIOC_Handler,
    (uint32_t)&GPIOD_Handler,
    (uint32_t)&GPIOE_Handler,
    (uint32_t)&UART0_Handler,
    (uint32_t)&UART1_Handler,
    (uint32_t)&SSI0_Handler,
    (uint32_t)&I2C0_Handler,
    (uint32_t)&PMW0_FAULT_Handler,
    (uint32_t)&PWM0_0_Handler,
    (uint32_t)&PWM0_1_Handler,
    (uint32_t)&PWM0_2_Handler,
    (uint32_t)&QEI0_Handler,
    (uint32_t)&ADC0SS0_Handler,
    (uint32_t)&ADC0SS1_Handler,
    (uint32_t)&ADC0SS2_Handler,
    (uint32_t)&ADC0SS3_Handler,
    (uint32_t)&WDT0_Handler,
    (uint32_t)&TIMER0A_Handler,
    (uint32_t)&TIMER0B_Handler,
    (uint32_t)&TIMER1A_Handler,
    (uint32_t)&TIMER1B_Handler,
    (uint32_t)&TIMER2A_Handler,
    (uint32_t)&TIMER2B_Handler,
    (uint32_t)&COMP0_Handler,
    (uint32_t)&COMP1_Handler,
    (uint32_t)&COMP2_Handler,
    (uint32_t)&SYSCTL_Handler,
    (uint32_t)&FLASH_Handler,
    (uint32_t)&GPIOF_Handler,
    (uint32_t)&GPIOG_Handler,
    (uint32_t)&GPIOH_Handler,
    (uint32_t)&UART2_Handler,
    (uint32_t)&SSI1_Handler,
    (uint32_t)&TIMER3A_Handler,
    (uint32_t)&TIMER3B_Handler,
    (uint32_t)&I2C1_Handler,
    (uint32_t)&QEI1_Handler,
    (uint32_t)&CAN0_Handler,
    (uint32_t)&CAN1_Handler,
    (uint32_t)&CAN2_Handler,
    0,
    (uint32_t)&HIB_Handler,
    (uint32_t)&USB0_Handler,
    (uint32_t)&PWM0_3_Handler,
    (uint32_t)&UDMA_Handler,
    (uint32_t)&UDMAERR_Handler,
    (uint32_t)&ADC1SS0_Handler,
    (uint32_t)&ADC1SS1_Handler,
    (uint32_t)&ADC1SS2_Handler,
    (uint32_t)&ADC1SS3_Handler,
    0,
    0,
    (uint32_t)&GPIOJ_Handler,
    (uint32_t)&GPIOK_Handler,
    (uint32_t)&GPIOL_Handler,
    (uint32_t)&SSI2_Handler,
    (uint32_t)&SSI3_Handler,
    (uint32_t)&UART3_Handler,
    (uint32_t)&UART4_Handler,
    (uint32_t)&UART5_Handler,
    (uint32_t)&UART6_Handler,
    (uint32_t)&UART7_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&I2C2_Handler,
    (uint32_t)&I2C3_Handler,
    (uint32_t)&TIMER4A_Handler,
    (uint32_t)&TIMER4B_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (uint32_t)&TIMER5A_Handler,
    (uint32_t)&TIMER5B_Handler,
    (uint32_t)&WTIMER0A_Handler,
    (uint32_t)&WTIMER0B_Handler,
    (uint32_t)&WTIMER1A_Handler,
    (uint32_t)&WTIMER1B_Handler,
    (uint32_t)&WTIMER2A_Handler,
    (uint32_t)&WTIMER2B_Handler,
    (uint32_t)&WTIMER3A_Handler,
    (uint32_t)&WTIMER3B_Handler,
    (uint32_t)&WTIMER4A_Handler,
    (uint32_t)&WTIMER4B_Handler,
    (uint32_t)&WTIMER5A_Handler,
    (uint32_t)&WTIMER5B_Handler,
    (uint32_t)&FPU_Handler,
    0,
    0,
    (uint32_t)&I2C4_Handler,
    (uint32_t)&I2C5_Handler,
    (uint32_t)&GPIOM_Handler,
    (uint32_t)&GPION_Handler,
    (uint32_t)&QEI2_Handler,
    0,
    0,
    (uint32_t)&GPIOP0_Handler,
    (uint32_t)&GPIOP1_Handler,
    (uint32_t)&GPIOP2_Handler,
    (uint32_t)&GPIOP3_Handler,
    (uint32_t)&GPIOP4_Handler,
    (uint32_t)&GPIOP5_Handler,
    (uint32_t)&GPIOP6_Handler,
    (uint32_t)&GPIOP7_Handler,
    (uint32_t)&GPIOQ0_Handler,
    (uint32_t)&GPIOQ1_Handler,
    (uint32_t)&GPIOQ2_Handler,
    (uint32_t)&GPIOQ3_Handler,
    (uint32_t)&GPIOQ4_Handler,
    (uint32_t)&GPIOQ5_Handler,
    (uint32_t)&GPIOQ6_Handler,
    (uint32_t)&GPIOQ7_Handler,
    (uint32_t)&GPIOR_Handler,
    (uint32_t)&GPIOS_Handler,
    (uint32_t)&PMW1_0_Handler,
    (uint32_t)&PWM1_1_Handler,
    (uint32_t)&PWM1_2_Handler,
    (uint32_t)&PWM1_3_Handler,
    (uint32_t)&PWM1_FAULT_Handler
};

// Declare external variables such as main, or symbols from the linker script
extern int main(void);
extern uint32_t __start_of_data;
extern uint32_t __end_of_data;
extern uint32_t __load_address_data;

void Reset_Handler() {
    SystemInit();

    // Copy data from flash to RAM
    uint32_t *data_source = &__start_of_data;

    for (uint32_t *data_destination = &__load_address_data; data_destination < &__end_of_data;) {
        *data_destination++ = *data_source++;
    }

    _start();
    main();
}

void Default_Handler(void) {
    while (1) {

    };
}