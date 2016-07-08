#include "unity_fixture.h"

#include "gpio/stm32f0_gpio.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(GPIOUsage);

TEST_GROUP_RUNNER(GPIOUsage) {
    RUN_TEST_CASE(GPIOUsage, PortConfiguration);
    RUN_TEST_CASE(GPIOUsage, PinConfiguration);
    RUN_TEST_CASE(GPIOUsage, ReadPort);
    RUN_TEST_CASE(GPIOUsage, ReadPin);
    RUN_TEST_CASE(GPIOUsage, CheckOutputPort);
    RUN_TEST_CASE(GPIOUsage, CheckOutputPin);
}

// Used to check addresses
static GPIOStruct global_test_gpio;
// Used to run calls
static GPIO test_gpio;

TEST_SETUP(GPIOUsage) {
    Mocksystem_memory_internals_Init();
    system_init_ExpectAndReturn((void *)GPIO_A_BASE_ADDRESS, sizeof(GPIOStruct), &global_test_gpio);
    test_gpio = gpio_create(0);
}

TEST_TEAR_DOWN(GPIOUsage) {
    gpio_destroy(test_gpio);
    Mocksystem_memory_internals_Verify();
    Mocksystem_memory_internals_Destroy();
}


TEST(GPIOUsage, PortConfiguration) {
    GPIOBaseConfig configuration = {
        .mode = OUTPUT,
        .type = OPEN_DRAIN,
        .speed = MED_SPEED,
        .pull = PULL_DOWN
    };

    UT_PTR_SET(system_write, mock_system_write_impl);
    // Type OPEN_DRAIN is 1, expanded gets you 0xFF ->
    // TODO: check that writing to reserved space doesn't break anything
    system_write_Expect(&(global_test_gpio.OTYPER), 0xFFFFFFFF);
    // Speed MED is 01, expanded gets you 0x55 ->
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x55555555);
    // pull down is 10, expanded gets you 0xAA ->
    system_write_Expect(&(global_test_gpio.PUPDR), 0xAAAAAAAA);
    // Mode output is 01, expanded gets you 0x55 ->
    system_write_Expect(&(global_test_gpio.MODER), 0x55555555);

    gpio_configure_port(test_gpio, configuration);
}

TEST(GPIOUsage, PinConfiguration) {
    GPIOPinConfig configuration = {
        .base_config = {
            .mode = OUTPUT,
            .type = OPEN_DRAIN,
            .speed = MED_SPEED,
            .pull = PULL_DOWN
        },
        .fxn = AF2
    };

    UT_PTR_SET(system_write, mock_system_write_impl);
    // Alternate function pin 1, AF2
    system_write_Expect(&(global_test_gpio.AFRL), 0x20);
    // Last check
    system_write_Expect(&(global_test_gpio.OTYPER), 0xFFFFFFFF);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x55555555);
    system_write_Expect(&(global_test_gpio.PUPDR), 0xAAAAAAAA);
    system_write_Expect(&(global_test_gpio.MODER), 0x55555555);

    gpio_configure_pin(test_gpio, configuration, 1);

    configuration.fxn = AF3;
    // Alternate function pin 10, AF3
    system_write_Expect(&(global_test_gpio.AFRH), 0x300);
    // Last check
    system_write_Expect(&(global_test_gpio.OTYPER), 0xFFFFFFFF);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x55555555);
    system_write_Expect(&(global_test_gpio.PUPDR), 0xAAAAAAAA);
    system_write_Expect(&(global_test_gpio.MODER), 0x55555555);

    gpio_configure_pin(test_gpio, configuration, 10);
}

TEST(GPIOUsage, ReadPort) {
    system_read_ExpectAndReturn(&(global_test_gpio.IDR), 0xBEEF);
    TEST_ASSERT_EQUAL_HEX32(0xBEEF, gpio_read_port(test_gpio));
}

TEST(GPIOUsage, ReadPin) {
    system_read_ExpectAndReturn(&(global_test_gpio.IDR), 0x01);
    TEST_ASSERT_EQUAL_HEX32(0x01, gpio_read_pin(test_gpio, 0));
    system_read_ExpectAndReturn(&(global_test_gpio.IDR), 0x01);
    TEST_ASSERT_EQUAL_HEX32(0x00, gpio_read_pin(test_gpio, 1));
    // NOTE: It only returns the bitmasked value, not a constant [1,0]
    system_read_ExpectAndReturn(&(global_test_gpio.IDR), 0x02);
    TEST_ASSERT_EQUAL_HEX32(0x02, gpio_read_pin(test_gpio, 1));
}

TEST(GPIOUsage, CheckOutputPort) {
    system_read_ExpectAndReturn(&(global_test_gpio.ODR), 0xDEAD);
    TEST_ASSERT_EQUAL_HEX32(0xDEAD, gpio_check_output_port(test_gpio));
}

TEST(GPIOUsage, CheckOutputPin) {
    system_read_ExpectAndReturn(&(global_test_gpio.ODR), 0x01);
    TEST_ASSERT_EQUAL_HEX32(0x01, gpio_check_output_pin(test_gpio, 0));
    system_read_ExpectAndReturn(&(global_test_gpio.ODR), 0x01);
    TEST_ASSERT_EQUAL_HEX32(0x00, gpio_check_output_pin(test_gpio, 1));
    // NOTE: It only returns the bitmasked value, not a constant [1,0]
    system_read_ExpectAndReturn(&(global_test_gpio.ODR), 0x02);
    TEST_ASSERT_EQUAL_HEX32(0x02, gpio_check_output_pin(test_gpio, 1));
}
