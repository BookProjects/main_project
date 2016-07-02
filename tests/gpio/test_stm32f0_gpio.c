#include "unity_fixture.h"
#include "gpio/stm32f0_gpio.h"

GPIO allocate_gpio() {
    return (GPIO)calloc(1, sizeof(GPIOStruct));
}

TEST_GROUP(GPIO);

TEST_GROUP_RUNNER(GPIO) {
    RUN_TEST_CASE(GPIO, Create);
}

TEST_SETUP(GPIO) {
}

TEST_TEAR_DOWN(GPIO) {
}

TEST(GPIO, Create) {
    uint32_t test_port = 0;
    GPIO test_gpio = gpio_create(test_port);
    TEST_FAIL_MESSAGE("Intentionally Fail");
}
