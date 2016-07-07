#include "unity_fixture.h"

#include "gpio/stm32f0_gpio.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(GPIO);

TEST_GROUP_RUNNER(GPIO) {
    RUN_TEST_CASE(GPIO, CreateAndDestroy);
}

TEST_SETUP(GPIO) {
    Mocksystem_memory_internals_Init();
}

TEST_TEAR_DOWN(GPIO) {
    Mocksystem_memory_internals_Verify();
    Mocksystem_memory_internals_Destroy();
}

// A private struct that takes the place of gpio
static GPIOStruct test_gpio;

TEST(GPIO, CreateAndDestroy) {
    uint32_t test_port = 0;
    system_init_ExpectAndReturn((void *)GPIO_A_BASE_ADDRESS, sizeof(GPIOStruct), &test_gpio);
    GPIO test_gpio = gpio_create(test_port);
    TEST_ASSERT_NOT_NULL(test_gpio);
    TEST_ASSERT_EQUAL_UINT32(OK, gpio_destroy(test_gpio));
}
