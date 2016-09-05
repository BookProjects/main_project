#include "processor/stm32f0/peripheral.h"

#include "gpio/stm32f0_gpio.h"
#include "clock/stm32f0_clock.h"


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
static const Peripheral port_peripheral_map[] = {
    [0] = IO_A,
    [1] = IO_B,
    [2] = IO_C,
    [3] = IO_D,
    [5] = IO_F
};


GPIO gpio_create(uint32_t gpio_port) {
    if(gpio_port < NUM_PORTS && gpio_port != PORT_E) {
        clock_power_peripheral(ON, port_peripheral_map[gpio_port]);
        return (GPIO) S_INIT(GPIOStruct, GPIOPorts[gpio_port]);
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
    S_DATA mask = 0xF << (pin << 2);
    if(pin < 8) {
        S_OW(self, AFRL, fxn, mask);
    } else if (pin < 16) {
        S_OW(self, AFRH, fxn, mask);
    } else {
        return NOT_OK;
    }
    S_DATA mask1 = BIT(pin);
    S_DATA mask2 = 0x33 << (pin << 1);
    S_OW(self, OTYPER, config.base_config.type << pin, mask1);
    S_OW(self, OSPEEDR, config.base_config.speed << (pin << 1), mask2);
    S_OW(self, PUPDR, config.base_config.pull << (pin << 1), mask2);
    S_OW(self, MODER, config.base_config.mode << (pin << 1), mask2);
    return OK;
}

err_t gpio_lock_configuration_for_pins(GPIO gpio, S_DATA pins) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    S_DATA off = (0xFFFF & pins);
    S_DATA on = 0x10000 | off;
    S_WR(self, LCKR, on);
    S_WR(self, LCKR, off);
    S_WR(self, LCKR, on);
    S_RD(self, LCKR);
    if(0x10000 & S_RD(self, LCKR)) {
        return OK;
    } else {
        return NOT_OK;
    }
}

S_DATA gpio_read_port(GPIO gpio) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    return S_RD(self, IDR);
}

S_DATA gpio_read_pin(GPIO gpio, GPIOPin pin) {
    S_DATA port_data = gpio_read_port(gpio);
    return port_data & BIT(pin);
}

S_DATA gpio_check_output_port(GPIO gpio) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    return S_RD(self, ODR);
}

S_DATA gpio_check_output_pin(GPIO gpio, GPIOPin pin) {
    S_DATA port_data = gpio_check_output_port(gpio);
    return port_data & BIT(pin);
}

err_t gpio_write_port(GPIO gpio, S_DATA data) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    S_WR(self, ODR, data);
    return OK;
}

err_t gpio_write_pin(GPIO gpio, GPIOPin pin, S_DATA on) {
    if(on) {
        return gpio_set_pin(gpio, pin);
    } else {
        return gpio_clear_pin(gpio, pin);
    }
}

err_t gpio_set_port(GPIO gpio, S_DATA data) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    // Never accidentally reset pins
    S_WR(self, BSRR, (0xFFFF) & data);
    return OK;
}

err_t gpio_set_pin(GPIO gpio, GPIOPin pin) {
    return gpio_set_port(gpio, BIT(pin));
}

err_t gpio_clear_port(GPIO gpio, S_DATA data) {
    GPIOStruct *self = (GPIOStruct *)gpio;
    // Never accidentally reset pins
    S_WR(self, BRR, data);
    return OK;
}

err_t gpio_clear_pin(GPIO gpio, GPIOPin pin) {
    return gpio_clear_port(gpio, BIT(pin));
}
