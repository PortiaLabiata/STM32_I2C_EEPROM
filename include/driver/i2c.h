#pragma once
#include "core/system.h"
#include "core/types.h"

#define IIC_WRITE_REQUEST   0b0UL
#define IIC_READ_REQUEST    0b1UL

/* Typedefs */


/* Macros */

#define EMERGENCY_STOP() do { \
    I2C1->CR1 &= ~I2C_CR1_START_Msk; \
    I2C1->CR1 |= I2C_CR1_STOP; \
} while(0)

/* Basic IO prototypes */

IIC_Status_t IIC_SendBytes(uint32_t addr, uint8_t *bytes, uint32_t size);
IIC_Status_t IIC_ReadBytes(uint32_t addr, uint8_t *bytes, uint32_t size);

/* Extended IO prototypes */

IIC_Status_t EEPROM_Write(uint32_t addr, uint16_t start_addr, uint8_t *bytes, uint32_t size);
IIC_Status_t EEPROM_Read(uint32_t addr, uint16_t start_addr, uint8_t *bytes, uint32_t size);