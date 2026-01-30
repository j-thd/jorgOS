/*

Since GPIO modules are used by many different modules, there needs to be a
central place to initialize/configure things once properly.

This also means GPIO modules are a shared resource of sorts, which there is
potential for conflict and bugs, if one does not keep track of what is being
used or what the state is properly. For now, this is a pet project, but in a
more professional project this needs serious structure to avoid mistakes.

Right now I can still oversee things, but I could implement some traceability to
ensure I keep oversight.

I could define the pins here, and then when a feature uses a pin elsewhere, I
can trace its usage.

*/


// MODULES
#define GPIO_D (1U << 3)
#define GPIO_E (1U << 4)
#define GPIO_F (1U << 5)

/* 
    PINS
*/

// GPIO D PINS

#define GPIO_D_0 (1U << 0) 
#define GPIO_D_1 (1U << 1)
#define GPIO_D_2 (1U << 2)
#define GPIO_D_3 (1U << 3)
#define GPIO_D_4 (1U << 4)
#define GPIO_D_5 (1U << 5)
#define GPIO_D_6 (1U << 6)
#define GPIO_D_7 (1U << 7)

// GPIO E PINS

#define GPIO_E_0 (1U << 0) 
#define GPIO_E_1 (1U << 1)
#define GPIO_E_2 (1U << 2)
#define GPIO_E_3 (1U << 3)
#define GPIO_E_4 (1U << 4)
#define GPIO_E_5 (1U << 5)
#define GPIO_E_6 (1U << 6)
#define GPIO_E_7 (1U << 7)

// GPIO F PINS
#define GPIO_F_0 (1U << 0) 
#define GPIO_F_1 (1U << 1)
#define GPIO_F_2 (1U << 2)
#define GPIO_F_3 (1U << 3)
#define GPIO_F_4 (1U << 4)
#define GPIO_F_5 (1U << 5)
#define GPIO_F_6 (1U << 6)
#define GPIO_F_7 (1U << 7)

/* 
    PORT MUX CONTROL REGISTERS
*/

// GPIO D PORT MUX CONTROL REGISTER

#define GPIO_D_0_PMC (0U) 
#define GPIO_D_1_PMC (4U)
#define GPIO_D_2_PMC (8U)
#define GPIO_D_3_PMC (12U)
#define GPIO_D_4_PMC (16U)
#define GPIO_D_5_PMC (20U)
#define GPIO_D_6_PMC (24U)
#define GPIO_D_7_PMC (28U)

// GPIO E PORT MUX CONTROL REGISTER

#define GPIO_E_0_PMC (0U) 
#define GPIO_E_1_PMC (4U)
#define GPIO_E_2_PMC (8U)
#define GPIO_E_3_PMC (12U)
#define GPIO_E_4_PMC (16U)
#define GPIO_E_5_PMC (20U)
#define GPIO_E_6_PMC (24U)
#define GPIO_E_7_PMC (28U)

// GPIO F PORT MUX CONTROL REGISTER

#define GPIO_F_0_PMC (0U) 
#define GPIO_F_1_PMC (4U)
#define GPIO_F_2_PMC (8U)
#define GPIO_F_3_PMC (12U)
#define GPIO_F_4_PMC (16U)
#define GPIO_F_5_PMC (20U)
#define GPIO_F_6_PMC (24U)
#define GPIO_F_7_PMC (28U)

void BSP_GPIO_init(void);