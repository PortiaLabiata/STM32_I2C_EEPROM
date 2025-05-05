#pragma once
#include <stm32f103x6.h>

/* Macros */

#define SET     (uint8_t)0x1
#define RESET   (uint8_t)0x0

/* Typedefs */


typedef enum {
    IIC_OK,
    IIC_CONFIG_ERROR,
    IIC_RUNT_AF_ERROR,
    IIC_RUNT_MS_ERROR,
    IIC_RUNT_OR_ERROR
} IIC_Status_t;