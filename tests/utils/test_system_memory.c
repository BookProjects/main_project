#include "unity_fixture.h"

#include "rcc/stm32f0_rcc.h"
#include "utils/system_memory.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(SystemMemory);

TEST_GROUP_RUNNER(SystemMemory) {
    RUN_TEST_CASE(SystemMemory, SetMask);
}

static RCCStruct test_rcc;

TEST_SETUP(SystemMemory) {
}

TEST_TEAR_DOWN(SystemMemory) {
    test_rcc = (RCCStruct) { 0 };
}

TEST(SystemMemory, SetMask) {
    RCCStruct * rcc_p= &test_rcc;
    UT_PTR_SET(system_write, mock_system_write_impl);

    system_write_Expect(&(test_rcc.AHBENR), BIT(1));
    S_SM(rcc_p, AHBENR, BIT(1), ON);

    system_write_Expect(&(test_rcc.AHBENR), BIT(2) | BIT(1));
    S_SM(rcc_p, AHBENR, BIT(2), ON);

    system_write_Expect(&(test_rcc.AHBENR), BIT(2));
    S_SM(rcc_p, AHBENR, BIT(1), OFF);

    system_write_Expect(&(test_rcc.AHBENR), TSC_EN | BIT(2));
    S_SM(rcc_p, AHBENR, get_peripheral_rcc_bit(TSC), ON);
    system_write_Expect(&(test_rcc.AHBENR), BIT(2));
    S_SM(rcc_p, AHBENR, get_peripheral_rcc_bit(TSC), OFF);
}
