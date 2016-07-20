#include "unity_fixture.h"

#include "clock/stm32f0_clock.h"
#include "mocks/mock_system_memory_internals.h"

TEST_GROUP(Clock);

TEST_GROUP_RUNNER(Clock) {
    RUN_TEST_CASE(Clock, ConfigureHSI);
}

static RCCStruct test_rcc;

TEST_SETUP(Clock) {
    Mocksystem_memory_internals_Init();
    system_init_ExpectAndReturn((void *)RCC_ADDRESS, sizeof(RCCStruct), &test_rcc);
    clock_create();
}

TEST_TEAR_DOWN(Clock) {
    Mocksystem_memory_internals_Verify();
    Mocksystem_memory_internals_Destroy();
}

TEST(Clock, ConfigureHSI) {
    clock_configure_HSI(1, 0xEF);
}
