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
        /* Initialization sequence
         * MODER -> 0 (All input)
         * OSPEEDR -> 0 (2MHZ, low speed)
         * PUPDR -> (each 2 bits is b'10 [pull-down])
         */
        S_WR(gpio, MODER, 0x00);
        S_WR(gpio, OSPEEDR, 0x00);
        S_WR(gpio, PUPDR, 0xAAAAAAAA);  // 0xA = 0b1010
        return (GPIO) gpio;
    } else {
        return NULL;
    }
}

err_t gpio_destroy(GPIO gpio) {
    S_DEL((GPIOStruct *) gpio);
    return OK;
}

err_t gpio_configure_port(GPIO gpio, GPIOBaseConfig config) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    S_WR(self, OTYPER, expand_nibble(config.type, NIB_1));
    S_WR(self, OSPEEDR, expand_nibble(config.speed, NIB_2));
    S_WR(self, PUPDR, expand_nibble(config.pull, NIB_2));
    S_WR(self, MODER, expand_nibble(config.mode, NIB_2));
    return OK;
}

err_t gpio_configure_pin(GPIO gpio, GPIOPinConfig config, GPIOPin pin) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    S_DATA fxn = config.fxn << (pin << 2);  // Shift left pin * 4 times, will wrap
    if(pin < 8) {
        S_WR(self, AFRL, fxn);
    } else if (pin < 16) {
        S_WR(self, AFRH, fxn);
    } else {
        return NOT_OK;
    }
    S_WR(self, OTYPER, expand_nibble(config.base_config.type, NIB_1));
    S_WR(self, OSPEEDR, expand_nibble(config.base_config.speed, NIB_2));
    S_WR(self, PUPDR, expand_nibble(config.base_config.pull, NIB_2));
    S_WR(self, MODER, expand_nibble(config.base_config.mode, NIB_2));
    return OK;
}

S_DATA gpio_read_port(GPIO gpio) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    return S_RD(self, IDR);
}
