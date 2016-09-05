#include "unity.h"
#include "stm32f0/test_basics.h"

void setUp() {
    start_unity_test();
}

void tearDown() {
    stop_unity_test();
}

int main() {
    setup_unity_output();
    UNITY_BEGIN();
    RUN_TEST(test_basic);
    return UNITY_END();
}
