#include "main.h"
#include "core/system.h"
#include "driver/i2c.h"

// I will put a prototype in here for now.
void Error_Handler(IIC_Status_t status);

int main(void) {
    Clock_Config();
    GPIO_Config();
    I2C_Config();

    uint8_t tx_buffer[8] = {0xD, 0xE, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF};
    //uint8_t rx_buffer[8];

    while (1) {
        IIC_Status_t status = EEPROM_Write(EEPROM_ADDR, 0, tx_buffer, 8);
        if (status != IIC_OK) {
            Error_Handler(status);
        }
        delay(100);
    }
}

void Error_Handler(IIC_Status_t status) {
    while (1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        delay(1000);
    }
}