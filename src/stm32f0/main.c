#include "gpio/gpio.h"
#include "gpio/stm32f0_gpio.h"
#include "utils/system_memory_internals.h"
#include "processor/stm32f0/peripheral.h"
#include "stm32f0/stm32f0xx.h"

void delay(int x);
void flash(GPIO gpio);

bool test_read_write();

bool test_read_write() {
    bool good = true;
    // Flash size should == 0x0040 (64kB)
    uint16_t flash_size = system_read(0x1FFFF7CC) & 0xFFFF;
    if(flash_size != 0x0040) {
        good = false;
    }
    // GPIO A MODER == 0x2800 0000 by default
    uint32_t gpio_a_moder = system_read(GPIO_A_BASE_ADDRESS);
    if(gpio_a_moder != 0x28000000) {
        good = false;
    }
    /*
    // Test Writing
    system_write(GPIO_A_BASE_ADDRESS, gpio_a_moder | 0xFFFF);
    gpio_a_moder = system_read(GPIO_A_BASE_ADDRESS);
    if(gpio_a_moder != 0x2800FFFF) {
        good = false;
    }
    */
    return good;
}


int main() {
    GPIO gpio_c = gpio_create(2);
    // @mjschulte pointed out FLASH -> RAM issue when not const
    const GPIOBaseConfig port_c_config = {
        .mode = OUTPUT,
        .type = PUSH_PULL,
        .speed = MED_SPEED,
        .pull = NO_PULL
    };
    gpio_configure_port(gpio_c, port_c_config);
    gpio_write_pin(gpio_c, 9, ON);
    gpio_write_pin(gpio_c, 8, ON);
    delay(200);
    flash();
    if(test_read_write() != true) {
        gpio_write_pin(gpio_c, 8, ON);
    }
    gpio_write_pin(gpio_c, 9, OFF);
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

void flash(GPIO gpio) {
    for(;;) {
        delay(500);
        gpio_write_pin(gpio_c, 9, OFF);
        delay(500);
        gpio_write_pin(gpio_c, 9, ON);
    }
}
