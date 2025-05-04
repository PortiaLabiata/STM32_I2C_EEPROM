#include "driver/i2c.h"

IIC_Status_t IIC_SendBytes(uint32_t addr, uint8_t *bytes, uint32_t size) {
    I2C1->CR1 &= ~I2C_CR1_POS_Msk;
    I2C1->CR1 |= I2C_CR1_ACK; // Prepare to ACK the next recieved byte
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB_Msk)) {
        __NOP();
    }
    (void) I2C1->SR1; // Read SR1 to clear SB bit
    I2C1->DR = ((addr << 1) | IIC_WRITE_REQUEST); // Send the address, including read/write bit.
    //volatile status = I2C1->SR1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR_Msk)) { // Wait until the transmission is complete
        __NOP();
    } // ACHTUNG!!! This section cannot be debugged, since the debugger actually reads both SR1
    // and SR2, and the interface considers this a reset condition for the ADDR bit and this loop
    // becomes infinite.
    /* This is needed to clear the ADDR bit */
    (void) I2C1->SR2;

    for (uint8_t *byte = bytes; byte - bytes < size; byte++) {
        I2C1->DR = (*byte); // Send the address, including read/write bit.
        while (!(I2C1->SR1 & I2C_SR1_TXE_Msk)) { // Wait until the transmission is complete
            __NOP();
        }
    }
    I2C1->CR1 |= I2C_CR1_STOP; // Generate stop condition
    return IIC_OK;
}

IIC_Status_t IIC_ReadBytes(uint32_t addr, uint8_t *bytes, uint32_t size) {
    I2C1->CR1 &= ~I2C_CR1_POS_Msk;
    I2C1->CR1 |= I2C_CR1_ACK; // Prepare to ACK the next recieved byte
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB_Msk)) {
        __NOP();
    }
    (void) I2C1->SR1; // Read SR1 to clear SB bit
    I2C1->DR = ((addr << 1) | IIC_READ_REQUEST); // Send the address, including read/write bit.
    while (!(I2C1->SR1 & I2C_SR1_ADDR_Msk)) { // Wait until the transmission is complete
        __NOP();
    } // ACHTUNG!!! This section cannot be debugged, since the debugger actually reads both SR1
    // and SR2, and the interface considers this a reset condition for the ADDR bit and this loop
    // becomes infinite.
    /* This is needed to clear the ADDR bit */
    (void) I2C1->SR2;

    uint8_t *byte = bytes;
    for (; (byte - bytes) < (size - 1); byte++) {
        while (!(I2C1->SR1 & I2C_SR1_RXNE_Msk)) {
            __NOP();
        }
        *byte = I2C1->DR & I2C_DR_DR_Msk;
    }
    I2C1->CR1 &= ~I2C_CR1_ACK_Msk; // Do not ACK the last byte to stop the transaction
    I2C1->CR1 |= I2C_CR1_STOP;
    while (!(I2C1->SR1 & I2C_SR1_RXNE_Msk)) {
        __NOP();
    }
    *byte = I2C1->DR & I2C_DR_DR_Msk;
    return IIC_OK;
}

