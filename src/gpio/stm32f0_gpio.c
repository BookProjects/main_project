#include "gpio/stm32f0_gpio.h"

GPIO gpio_create(uint32_t gpio_port) {
    return (GPIO) S_INIT(GPIOStruct, GPIO_A_BASE_ADDRESS);
}

err_t gpio_destroy(GPIO gpio) {
    S_DEL((GPIOStruct *) gpio);
    return OK;
}
