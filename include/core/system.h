#pragma once
#include <stm32f103x6.h>

#define HSI_FREQ                8000000 // HSI frequency in Hz
#define HSE_FREQ                0       // Since I don't have HSE
#define NVIC_PRIORITY_GROUPING  4

#define IIC_F_SM                100000UL
#define IIC_FHIGH_SM            (uint32_t)(2*IIC_F_SM)
#define IIC_CCR                 (uint32_t)(PCLK1_FREQ/IIC_FHIGH_SM)

#define SYSCLK_FREQ _sysclk_freq
#define AHB_FREQ    _ahb_freq
#define PCLK2_FREQ  _pclk2_freq
#define PCLK1_FREQ  _pclk1_freq

/* Global variables */

extern uint32_t _sysclk_freq;
extern uint32_t _ahb_freq;
extern uint32_t _pclk2_freq;
extern uint32_t _pclk1_freq;
extern volatile uint32_t _current_ticks;

/* Configraton functions */

void Clock_Config(void);
void GPIO_Config(void);
void I2C_Config(void);

/* Information functions */

uint32_t Get_CurrentTicks(void);

/* Utilitary functions */

void delay(uint32_t ms);