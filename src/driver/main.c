#include "main.h"
#include "core/system.h"
#include "driver/i2c.h"

int main(void) {
    Clock_Config();
    GPIO_Config();
    I2C_Config();

    uint8_t tx_buffer[8] = {0xD, 0xE, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF};
    uint8_t rx_buffer[8];

    while (1) {
        //IIC_SendBytes(EEPROM_ADDR, tx_buffer, 8);
        EEPROM_Write(EEPROM_ADDR, 0, tx_buffer, 8);
        delay(10);
        EEPROM_Read(EEPROM_ADDR, 0, rx_buffer, 8);
        delay(100);
    }
}