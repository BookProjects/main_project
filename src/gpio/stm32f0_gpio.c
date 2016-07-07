#include "gpio/stm32f0_gpio.h"

static const uintptr_t GPIOPorts[] = {
    GPIO_A_BASE_ADDRESS,
    GPIO_B_BASE_ADDRESS,
    GPIO_C_BASE_ADDRESS,
    GPIO_D_BASE_ADDRESS,
    (uintptr_t) NULL,
    GPIO_F_BASE_ADDRESS
};

static const uint32_t NUM_PORTS = 6;
static const uint32_t PORT_E = 4;  // Unused GPIO port in STM32F0

GPIO gpio_create(uint32_t gpio_port) {
    if(gpio_port < NUM_PORTS && gpio_port != PORT_E) {
        GPIOStruct *gpio = S_INIT(GPIOStruct, GPIOPorts[gpio_port]);
        S_WR(gpio, MODER, 0x20);
        return (GPIO) gpio;
    } else {
        return NULL;
    }
}

err_t gpio_destroy(GPIO gpio) {
    S_DEL((GPIOStruct *) gpio);
    return OK;
}
