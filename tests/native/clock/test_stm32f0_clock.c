#include "unity_fixture.h"

#include "clock/stm32f0_clock.h"
#include "flash/stm32f0_flash.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(Clock);

TEST_GROUP_RUNNER(Clock) {
    RUN_TEST_CASE(Clock, ConfigureHSI);
    RUN_TEST_CASE(Clock, PowerPeripheral);
}

static RCCStruct test_rcc;
static FlashStruct test_flash;

TEST_SETUP(Clock) {
    Mocksystem_memory_internals_Init();
    system_init_ExpectAndReturn((void *)RCC_ADDRESS, sizeof(RCCStruct), &test_rcc);
    system_init_ExpectAndReturn((void *)FLASH_ADDRESS, sizeof(FlashStruct), &test_flash);
    clock_create();
}

TEST_TEAR_DOWN(Clock) {
    test_rcc = (RCCStruct) { 0 };
    test_flash = (FlashStruct) { 0 };
    Mocksystem_memory_internals_Verify();
    Mocksystem_memory_internals_Destroy();
}

TEST(Clock, ConfigureHSI) {
    TEST_ASSERT_EQUAL_UINT32(NOT_OK, clock_configure_HSI(ON, 0xEF));
    test_rcc.CR |= HSI_RDY;
    TEST_ASSERT_EQUAL_UINT32(OK, clock_configure_HSI(ON, 0xEF));
}

TEST(Clock, PowerPeripheral) {
    UT_PTR_SET(system_write, mock_system_write_impl);

    system_write_Expect(&(test_rcc.AHBENR), IOA_EN);
    clock_power_peripheral(ON, IO_A);

    system_write_Expect(&(test_rcc.AHBENR), IOA_EN | IOB_EN);
    clock_power_peripheral(ON, IO_B);

    system_write_Expect(&(test_rcc.AHBENR), IOB_EN);
    clock_power_peripheral(OFF, IO_A);
}
