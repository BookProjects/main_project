#include "gpio/stm32f0_gpio.h"

GPIO gpio_create(uint32_t gpio_port) {
    GPIOStruct * value = S_INIT(GPIOStruct, GPIO_A_BASE_ADDRESS);
    S_WR(value, MODER, 0x10);
    S_RD(value, IDR);
    return (GPIO) value;
}