#include "main.h"
#include "core/system.h"
#include "driver/i2c.h"

int main(void) {
    Clock_Config();
    GPIO_Config();
    I2C_Config();

    //uint8_t tx_buffer[8] = {0xD, 0xE, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF};
    uint8_t tx_buffer[2] = {0x0, 0x0};
    uint8_t rx_buffer[8];

    while (1) {
        //IIC_SendBytes(EEPROM_ADDR, tx_buffer, 8);
        IIC_SendBytes(EEPROM_ADDR, tx_buffer, 2); // Send the address
        IIC_ReadBytes(EEPROM_ADDR, rx_buffer, 8); // Read the EEPROM contents
        delay(100);
    }
}