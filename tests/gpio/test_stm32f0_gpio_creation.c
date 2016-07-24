#include "unity_fixture.h"

#include "clock/stm32f0_clock.h"
#include "gpio/stm32f0_gpio.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(GPIO);

TEST_GROUP_RUNNER(GPIO) {
    RUN_TEST_CASE(GPIO, CreateAndDestroy);
    RUN_TEST_CASE(GPIO, CreateGoodPorts);
    RUN_TEST_CASE(GPIO, TryCreatingBadPorts);
    RUN_TEST_CASE(GPIO, ProperInitialization);
}

static RCCStruct test_rcc;

TEST_SETUP(GPIO) {
    Mocksystem_memory_internals_Init();
    system_init_ExpectAndReturn((void *)RCC_ADDRESS, sizeof(RCCStruct), &test_rcc);
    clock_create();
}

TEST_TEAR_DOWN(GPIO) {
    test_rcc = (RCCStruct) { 0 };
    Mocksystem_memory_internals_Verify();
    Mocksystem_memory_internals_Destroy();
}

// A private struct that takes the place of gpio
static GPIOStruct global_test_gpio;

static void system_init_expect(uintptr_t address) {
    system_init_ExpectAndReturn((void *)address, sizeof(GPIOStruct), &global_test_gpio);
}

TEST(GPIO, CreateAndDestroy) {
    uint32_t test_port = 0;
    system_init_expect(GPIO_A_BASE_ADDRESS);
    GPIO test_gpio = gpio_create(test_port);
    TEST_ASSERT_NOT_NULL(test_gpio);
    TEST_ASSERT_EQUAL_UINT32(OK, gpio_destroy(test_gpio));
}

TEST(GPIO, CreateGoodPorts) {
    uint32_t good_ports[] = {0, 1, 2, 3, 5};
    uintptr_t port_addresses[] = {
        GPIO_A_BASE_ADDRESS,
        GPIO_B_BASE_ADDRESS,
        GPIO_C_BASE_ADDRESS,
        GPIO_D_BASE_ADDRESS,
        GPIO_F_BASE_ADDRESS
    };
    for(int i = 0; i < 5; ++i) {
        system_init_expect(port_addresses[i]);
        GPIO test_gpio = gpio_create(good_ports[i]);
        TEST_ASSERT_NOT_NULL(test_gpio);
        gpio_destroy(test_gpio);
    }
}

TEST(GPIO, TryCreatingBadPorts) {
    uint32_t bad_ports[] = {4, 6, 7, 200, (uint32_t) -1};
    for(int i = 0; i < 5; ++i) {
        GPIO test_gpio = gpio_create(bad_ports[i]);
        TEST_ASSERT_NULL(test_gpio);
        gpio_destroy(test_gpio);
    }
}

TEST(GPIO, ProperInitialization) {
    // Ensure nothing is written to
    system_init_expect(GPIO_A_BASE_ADDRESS);
    UT_PTR_SET(system_write, mock_system_write_impl);
    system_write_Expect(&(test_rcc.AHBENR), IOA_EN);
    GPIO test_gpio = gpio_create(0);
    gpio_destroy(test_gpio);
}
