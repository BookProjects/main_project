#include "stm32f0/unity_config.h"
#include "gpio/gpio.h"
#include "gpio/stm32f0_gpio.h"

static char answer[4] = "NO\n";
static GPIO gpio_output = NULL;


int cross_unity_output_char(int a) {
    answer[0] = answer[1];
    answer[1] = answer[2];
    answer[2] = a;
    return 0;
}

void cross_unity_output_complete() {
    if(strcmp(answer, "OK\n") == 0) {
        answer[0] = 'A';
        // Green is good
        gpio_write_pin(gpio_output, 9, ON);
    } else {
        answer[0] = 'F';
        // Blue is bad
        gpio_write_pin(gpio_output, 8, ON);
    }
}

void setup_unity_output() {
    gpio_output = gpio_create(2);
    GPIOBaseConfig port_c_config = {
        .mode = OUTPUT,
        .type = PUSH_PULL,
        .speed = MED_SPEED,
        .pull = NO_PULL
    };
    gpio_configure_port(gpio_output, port_c_config);
    gpio_write_pin(gpio_output, 9, OFF);
    gpio_write_pin(gpio_output, 8, OFF);
}
