#include "unity_fixture.h"

#include "clock/stm32f0_clock.h"
#include "gpio/stm32f0_gpio.h"
#include "utils/errors.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(GPIOUsage);

TEST_GROUP_RUNNER(GPIOUsage) {
    RUN_TEST_CASE(GPIOUsage, PortConfiguration);
    RUN_TEST_CASE(GPIOUsage, PinConfiguration);
    RUN_TEST_CASE(GPIOUsage, LockConfiguration);
    RUN_TEST_CASE(GPIOUsage, LockConfigurationOverload);
    RUN_TEST_CASE(GPIOUsage, LockConfigurationFail);

    RUN_TEST_CASE(GPIOUsage, ReadPort);
    RUN_TEST_CASE(GPIOUsage, ReadPin);

    RUN_TEST_CASE(GPIOUsage, CheckOutputPort);
    RUN_TEST_CASE(GPIOUsage, CheckOutputPin);

    RUN_TEST_CASE(GPIOUsage, WritePort);
    RUN_TEST_CASE(GPIOUsage, WritePin);
    RUN_TEST_CASE(GPIOUsage, SetPort);
    RUN_TEST_CASE(GPIOUsage, SetPin);
    RUN_TEST_CASE(GPIOUsage, ClearPort);
    RUN_TEST_CASE(GPIOUsage, ClearPin);
}

// Used to check addresses
static GPIOStruct global_test_gpio;
// Used to run calls
static GPIO test_gpio;
static RCCStruct test_rcc;

static S_DATA default_system_read_impl(S_DATA *src) {
    return *src;
}

TEST_SETUP(GPIOUsage) {
    Mocksystem_memory_internals_Init();
    system_init_ExpectAndReturn((void *)RCC_ADDRESS, sizeof(RCCStruct), &test_rcc);
    clock_create();
    system_init_ExpectAndReturn((void *)GPIO_A_BASE_ADDRESS, sizeof(GPIOStruct), &global_test_gpio);
    test_gpio = gpio_create(0);
    UT_PTR_SET(system_write, mock_system_write_impl);
    UT_PTR_SET(system_read, mock_system_read_impl);
}

TEST_TEAR_DOWN(GPIOUsage) {
    gpio_destroy(test_gpio);
    global_test_gpio = (GPIOStruct) { 0 };
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
    UT_PTR_SET(system_read, default_system_read_impl);
    GPIOPinConfig configuration = {
        .base_config = {
            .mode = OUTPUT,
            .type = OPEN_DRAIN,
            .speed = MED_SPEED,
            .pull = PULL_DOWN
        },
        .fxn = AF2
    };

    // Alternate function pin 1, AF2
    system_write_Expect(&(global_test_gpio.AFRL), 0x20);
    system_write_Expect(&(global_test_gpio.OTYPER), 0x02);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x04);
    system_write_Expect(&(global_test_gpio.PUPDR), 0x08);
    system_write_Expect(&(global_test_gpio.MODER), 0x04);

    gpio_configure_pin(test_gpio, configuration, 1);

    // Alternate function pin 1, AF1
    configuration.fxn = AF1;
    system_write_Expect(&(global_test_gpio.AFRL), 0x10);
    system_write_Expect(&(global_test_gpio.OTYPER), 0x02);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x04);
    system_write_Expect(&(global_test_gpio.PUPDR), 0x08);
    system_write_Expect(&(global_test_gpio.MODER), 0x04);

    gpio_configure_pin(test_gpio, configuration, 1);

    // Alternate function pin 10, AF3
    configuration.fxn = AF3;
    // Since just configuring pin, don't want to overwrite prev. configurations
    system_write_Expect(&(global_test_gpio.AFRH), 0x300);
    // b'100 0000 0010
    system_write_Expect(&(global_test_gpio.OTYPER), 0x0402);
    //   10   9  8   7  6   5  4   3  2   1  0
    // b'01, 00 00, 00 00 ,00 00, 00 00, 01 00
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x100004);
    system_write_Expect(&(global_test_gpio.PUPDR),   0x200008);
    system_write_Expect(&(global_test_gpio.MODER),   0x100004);

    gpio_configure_pin(test_gpio, configuration, 10);

    // AF1, pin 11
    configuration.fxn = AF1;
    system_write_Expect(&(global_test_gpio.AFRH), 0x01300);
    system_write_Expect(&(global_test_gpio.OTYPER), 0x0C02);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x500004);
    system_write_Expect(&(global_test_gpio.PUPDR),   0xA00008);
    system_write_Expect(&(global_test_gpio.MODER),   0x500004);
    gpio_configure_pin(test_gpio, configuration, 11);

    // pin 11
    configuration.base_config.mode = ALT;
    configuration.base_config.speed = LOW_SPEED;
    configuration.base_config.pull = PULL_UP;
    system_write_Expect(&(global_test_gpio.AFRH), 0x01300);
    system_write_Expect(&(global_test_gpio.OTYPER), 0x0C02);
    system_write_Expect(&(global_test_gpio.OSPEEDR), 0x100004);
    system_write_Expect(&(global_test_gpio.PUPDR),   0x600008);
    system_write_Expect(&(global_test_gpio.MODER),   0x900004);
    gpio_configure_pin(test_gpio, configuration, 11);
}

static void expect_lock_sequence(S_DATA pins, S_DATA final_value) {
    S_DATA off_and_pins = (0xFFFF & pins);
    S_DATA on_and_pins = 0x10000 | off_and_pins;
    system_write_Expect(&(global_test_gpio.LCKR),         on_and_pins);
    system_write_Expect(&(global_test_gpio.LCKR),         off_and_pins);
    system_write_Expect(&(global_test_gpio.LCKR),         on_and_pins);
    system_read_ExpectAndReturn(&(global_test_gpio.LCKR), off_and_pins);
    system_read_ExpectAndReturn(&(global_test_gpio.LCKR), final_value | off_and_pins);
}

TEST(GPIOUsage, LockConfiguration) {
    // Successful
    expect_lock_sequence(0xBEEF, 0x10000);
    TEST_ASSERT_EQUAL_UINT32(
        OK, gpio_lock_configuration_for_pins(test_gpio,    0xBEEF));
}

TEST(GPIOUsage, LockConfigurationOverload) {
    // Overload
    expect_lock_sequence(0xBEEF, 0x10000);
    TEST_ASSERT_EQUAL_UINT32(
        OK, gpio_lock_configuration_for_pins(test_gpio,    0xDEADBEEF));
}

TEST(GPIOUsage, LockConfigurationFail) {
    // Failing
    expect_lock_sequence(0xBEEF, 0);
    TEST_ASSERT_EQUAL_UINT32(
        NOT_OK, gpio_lock_configuration_for_pins(test_gpio,    0xBEEF));
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

TEST(GPIOUsage, WritePort) {
    system_write_Expect(&(global_test_gpio.ODR), 0xBEEF);
    gpio_write_port(test_gpio, 0xBEEF);
}

TEST(GPIOUsage, WritePin) {
    system_write_Expect(&(global_test_gpio.BSRR), 0x0100);
    gpio_write_pin(test_gpio, 8, 1);
    system_write_Expect(&(global_test_gpio.BSRR), 0x0100);
    gpio_write_pin(test_gpio, 8, 0x55);
    system_write_Expect(&(global_test_gpio.BRR), 0x0100);
    gpio_write_pin(test_gpio, 8, 0);
}

TEST(GPIOUsage, SetPort) {
    system_write_Expect(&(global_test_gpio.BSRR), 0xDEAD);
    gpio_set_port(test_gpio, 0xDEAD);
    // Don't let it overwrite
    system_write_Expect(&(global_test_gpio.BSRR), 0xBEEF);
    gpio_set_port(test_gpio, 0xDEADBEEF);
}

TEST(GPIOUsage, SetPin) {
    system_write_Expect(&(global_test_gpio.BSRR), 0x0100);
    gpio_set_pin(test_gpio, 8);
}

TEST(GPIOUsage, ClearPort) {
    system_write_Expect(&(global_test_gpio.BRR), 0xDEAD);
    gpio_clear_port(test_gpio, 0xDEAD);
}

TEST(GPIOUsage, ClearPin) {
    system_write_Expect(&(global_test_gpio.BRR), 0x0100);
    gpio_clear_pin(test_gpio, 8);
}
