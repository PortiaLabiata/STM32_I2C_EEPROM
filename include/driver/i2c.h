#pragma once
#include "core/system.h"
#include "core/types.h"

#define IIC_WRITE_REQUEST   0b0UL
#define IIC_READ_REQUEST    0b1UL

/* Typedefs */

typedef enum {
    IIC_OK,
    IIC_CONFIG_ERROR,
    IIC_RUNT_ERROR
} IIC_Status_t;

/* Basic IO prototypes */

IIC_Status_t IIC_SendBytes(uint32_t addr, uint8_t *bytes, uint32_t size);
IIC_Status_t IIC_ReadBytes(uint32_t addr, uint8_t *bytes, uint32_t size);

/* Extended IO prototypes */

IIC_Status_t EEPROM_Write(uint32_t addr, uint16_t start_addr, uint8_t *bytes, uint32_t size);
IIC_Status_t EEPROM_Read(uint32_t addr, uint16_t start_addr, uint8_t *bytes, uint32_t size);