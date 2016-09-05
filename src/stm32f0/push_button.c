#include "gpio/gpio.h"
#include <setjmp.h>

void delay(int);

int main() {
    GPIO gpio_a = gpio_create(0);
    GPIO gpio_c = gpio_create(2);
    const GPIOBaseConfig port_c_config = {
        .mode = OUTPUT,
        .type = PUSH_PULL,
        .speed = MED_SPEED,
        .pull = NO_PULL
    };
    const GPIOPinConfig pin_a0_config = {
        .base_config = {
            .mode = INPUT,
            .type = PUSH_PULL,
            .speed = MED_SPEED,
            .pull = PULL_DOWN
        },
        .fxn = AF0
    };
    gpio_configure_port(gpio_c, port_c_config);
    gpio_configure_pin(gpio_a, pin_a0_config, 0);

    gpio_write_pin(gpio_c, 8, ON);
    jmp_buf env;
    int res = setjmp(env);
    // for(;;) {
        delay(500);
        gpio_write_pin(gpio_c, 9, OFF);
        gpio_write_pin(gpio_c, 8, gpio_read_pin(gpio_a, 0));
        delay(500);
        gpio_write_pin(gpio_c, 9, ON);
        gpio_write_pin(gpio_c, 8, gpio_read_pin(gpio_a, 0));
    // }
    longjmp(env, 1);

    gpio_destroy(gpio_c);
    return 0;
}

void delay(int x) {
    volatile int k;
    for(volatile int i = 0; i < x; ++i) {
        for(volatile int j = 0; j < 1000; ++j) {
            k++;
        }
    }
}
