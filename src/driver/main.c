#include "main.h"
#include "core/system.h"

int main(void) {
    Clock_Config();
    GPIO_Config();

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        delay(1000);
    }
}