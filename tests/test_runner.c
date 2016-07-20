#include "unity_fixture.h"

static void RunAllTests(void) {
    RUN_TEST_GROUP(GPIO);
    RUN_TEST_GROUP(GPIOUsage);
    RUN_TEST_GROUP(Clock);
}

int main(int argc, const char *argv[]) {
    return UnityMain(argc, argv, RunAllTests);
}
