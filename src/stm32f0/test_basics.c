#include "stm32f0/test_basics.h"

#include "unity.h"
#include "utils/system_memory_internals.h"
#include "processor/stm32f0/peripheral.h"
#include "stm32f0/stm32f0xx.h"
#include "gpio/stm32f0_gpio.h"

void test_basic() {
    // Flash size should == 0x0040 (64kB)
    uint16_t flash_size = system_read(0x1FFFF7CC) & 0xFFFF;
    TEST_ASSERT_EQUAL_UINT16(0x0040, flash_size);
    // GPIO A MODER == 0x2800 0000 by default
    uint32_t gpio_a_moder = system_read(GPIO_A_BASE_ADDRESS);
    TEST_ASSERT_EQUAL_UINT32(0x28000000, gpio_a_moder);
}
