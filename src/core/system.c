#include "core/system.h"

uint32_t _sysclk_freq = 0;
uint32_t _ahb_freq = 0;
uint32_t _pclk2_freq = 0;
uint32_t _pclk1_freq = 0;

volatile uint32_t _current_ticks = 0;

/* System information functions */

/**
 * \brief Calculates SYSCLK frequency based on RCC registers content. Is supposed to 
 * only run once at the start of the program.
 * \returns Frequency in hertz.
 */
static uint32_t Get_SYSCLK_Freq(void) {
    uint32_t pmul;
    switch (RCC->CFGR & (3UL << RCC_CFGR_SWS_Pos)) {
        case RCC_CFGR_SWS_HSI:
            return HSI_FREQ;
        case RCC_CFGR_SWS_HSE:
            return HSE_FREQ;
        case RCC_CFGR_SWS_PLL:
            pmul = (RCC->CFGR >> RCC_CFGR_PLLMULL_Pos) & 0b1111;
            return (pmul + 2)*HSI_FREQ/2;
        default:
            return 0; // Something went horribly wrong
    }
}

/**
 * \brief Calculates AHB frequency based on RCC registers content. Is supposed to 
 * only run once at the start of the program.
 * \returns Frequency in hertz.
 */
static uint32_t Get_AHB_Freq(void) {
    switch (RCC->CFGR & (0b1111 << RCC_CFGR_HPRE_Pos)) {
        case RCC_CFGR_HPRE_DIV1:
            return SYSCLK_FREQ / 1;
        case RCC_CFGR_HPRE_DIV2:
            return SYSCLK_FREQ / 2;
        case RCC_CFGR_HPRE_DIV4:
            return SYSCLK_FREQ / 4;
        case RCC_CFGR_HPRE_DIV8:
            return SYSCLK_FREQ / 8;
        case RCC_CFGR_HPRE_DIV16:
            return SYSCLK_FREQ / 16;
        case RCC_CFGR_HPRE_DIV64:
            return SYSCLK_FREQ / 64;
        case RCC_CFGR_HPRE_DIV128:
            return SYSCLK_FREQ / 128;
        case RCC_CFGR_HPRE_DIV256:
            return SYSCLK_FREQ / 256;
        case RCC_CFGR_HPRE_DIV512:
            return SYSCLK_FREQ / 512;
        default:
            return 0; // Something went horribly wrong
    }
}

/**
 * \brief Calculates PCLK2 frequency based on RCC registers content. Is supposed to 
 * only run once at the start of the program.
 * \returns Frequency in hertz.
 */
static uint32_t Get_PCLK2_Freq(void) {
    switch (RCC->CFGR & (0b111 << RCC_CFGR_PPRE2_Pos)) {
        case RCC_CFGR_PPRE2_DIV1:
            return AHB_FREQ / 1;
        case RCC_CFGR_PPRE2_DIV2:
            return AHB_FREQ / 2;
        case RCC_CFGR_PPRE2_DIV4:
            return AHB_FREQ / 4;
        case RCC_CFGR_PPRE2_DIV8:
            return AHB_FREQ / 8;
        case RCC_CFGR_PPRE2_DIV16:
            return AHB_FREQ / 16;
        default:
            return 0;
    }
}

/**
 * \brief Calculates PCLK1 frequency based on RCC registers content. Is supposed to 
 * only run once at the start of the program.
 * \returns Frequency in hertz.
 */
static uint32_t Get_PCLK1_Freq(void) {
    switch (RCC->CFGR & (0b111 << RCC_CFGR_PPRE1_Pos)) {
        case RCC_CFGR_PPRE1_DIV1:
            return AHB_FREQ / 1;
        case RCC_CFGR_PPRE1_DIV2:
            return AHB_FREQ / 2;
        case RCC_CFGR_PPRE1_DIV4:
            return AHB_FREQ / 4;
        case RCC_CFGR_PPRE1_DIV8:
            return AHB_FREQ / 8;
        case RCC_CFGR_PPRE1_DIV16:
            return AHB_FREQ / 16;
        default:
            return 0;
    }
}

/**
 * \brief Returns current number of SysTick ticks elapsed from the startup.
 */
uint32_t Get_CurrentTicks(void) {
    return _current_ticks;
}

/**
 * \brief Blocking delay, using SysTick.
 * \param[in] ms Delay length, ms.
 */
void delay(uint32_t ms) {
    uint32_t ms_start = Get_CurrentTicks();
    while (Get_CurrentTicks() - ms_start < ms) {
        __NOP();
    }
}

/**
 * \brief Low-level RCC configuration.
 */
void Clock_Config(void) {
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY_Msk)) {
        __NOP();
    }

    RCC->CFGR |= (RCC_CFGR_PLLMULL12 | RCC_CFGR_SW_PLL | RCC_CFGR_PPRE1_DIV2);
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY_Msk)) {
        __NOP();
    }
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {
        __NOP();
    }

    FLASH->ACR |= FLASH_ACR_LATENCY_1; // Enable flash latency with 2 wait-states

    _sysclk_freq = Get_SYSCLK_Freq();
    _ahb_freq = Get_AHB_Freq();
    _pclk1_freq = Get_PCLK1_Freq();
    _pclk2_freq = Get_PCLK2_Freq();

    SysTick_Config(PCLK2_FREQ / 1000);
    NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUPING);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUPING, 0, 0));
    NVIC_EnableIRQ(SysTick_IRQn);
}

void GPIO_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13_Msk;
    GPIOC->CRH |= (GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1);
}

void I2C_Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* Configure AFIO */
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_Msk | GPIO_CRL_CNF7_Msk);
    GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1);
    GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1);

    I2C1->CR1 |= I2C_CR1_ACK;
    // No need to enable clock stretching or general calls, since reset value is 0
    I2C1->CR2 |= ((PCLK1_FREQ/1000000) << I2C_CR2_FREQ_Pos); // Set clock frequency to PCLK1 frequency
    I2C1->CCR = IIC_CCR; // CCR = THigh / TPCLK1
    /* Honestly, I have no earthly idea, what this thing does. It says, that it sets the rise period
    for the SCL line, but I have no idea, how. Like, there is no official info, even on forums.
    I am writing this in, like, a middle of the night, and official ST docs are so friging stupid. 
    I think it is written by a bunch of interns with a total brain capacity of, well, 
    less than me, which is a goddamn world record.  */
    I2C1->TRISE = (PCLK1_FREQ / 1000000) + 1;
    I2C1->CR1 |= I2C_CR1_PE; // Device enable
}