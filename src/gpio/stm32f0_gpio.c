#include "gpio/stm32f0_gpio.h"

static const uintptr_t GPIOPorts[] = {
    GPIO_A_BASE_ADDRESS,
    GPIO_B_BASE_ADDRESS,
    GPIO_C_BASE_ADDRESS,
    GPIO_D_BASE_ADDRESS,
    (uintptr_t) NULL,
    GPIO_F_BASE_ADDRESS
};

GPIO gpio_create(uint32_t gpio_port) {
    return (GPIO) S_INIT(GPIOStruct, GPIOPorts[gpio_port]);
}

err_t gpio_destroy(GPIO gpio) {
    S_DEL((GPIOStruct *) gpio);
    return OK;
}
