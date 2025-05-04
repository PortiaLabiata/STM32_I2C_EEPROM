#pragma once
#include "core/system.h"
#include "core/types.h"

#define IIC_WRITE   0b0UL

/* Typedefs */

typedef enum {
    IIC_OK,
    IIC_CONFIG_ERROR,
    IIC_RUNT_ERROR
} IIC_Status_t;

/* Prototypes */

IIC_Status_t IIC_SendBytes(uint32_t addr, uint8_t *bytes, uint32_t size);