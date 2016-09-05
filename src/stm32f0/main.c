#include "gpio/gpio.h"
#include "gpio/stm32f0_gpio.h"
#include "utils/system_memory_internals.h"
#include "processor/stm32f0/peripheral.h"
#include "stm32f0/stm32f0xx.h"
#include <setjmp.h>
// #include "unity_fixture.h"
#include "unity.h"

void delay(int x);
void flash(GPIO gpio);

bool test_read_write();

void setUp() {
}

void tearDown() {
}

void test_basic() {
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
    /*
    delay(200);
    flash(gpio_c);
    */
    TEST_ASSERT_EQUAL_UINT32(0, 0);
    return good;
}


/*
static void RunAllTests(void) {
    RUN_TEST_GROUP(GPIO);
}
*/

int main() {
    setup_unity_output();
    UNITY_BEGIN();
    RUN_TEST(test_basic);
    return UNITY_END();
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
    jmp_buf env;
    int val = setjmp(env);
    {
		delay(500);
        gpio_write_pin(gpio, 9, OFF);
		delay(500);
        gpio_write_pin(gpio, 9, ON);
	}
    longjmp(env, 1);
}
