#include "gpio/stm32f0_gpio.h"
#include "utils/system_memory.h"

GPIO gpio_create(uint32_t gpio_port) {
    GPIOStruct * value = S_INIT(GPIOStruct, GPIO_A_BASE_ADDRESS);
    value->MODER = 0x10;
    return (GPIO) value;
}
