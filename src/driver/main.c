#include "main.h"
#include "core/system.h"
#include "driver/i2c.h"

int main(void) {
    Clock_Config();
    GPIO_Config();
    I2C_Config();

    uint8_t tx_buffer[1] = {0xD};

    while (1) {
        IIC_SendBytes(EEPROM_ADDR, tx_buffer, 1);
        delay(100);
    }
}