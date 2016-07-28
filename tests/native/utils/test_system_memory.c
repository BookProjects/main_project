#include "unity_fixture.h"

#include "utils/system_memory.h"
#include "mocks/mock_system_memory_internals.h"

#include <setjmp.h>

TEST_GROUP(SystemMemory);

TEST_GROUP_RUNNER(SystemMemory) {
    RUN_TEST_CASE(SystemMemory, SetMask);
    RUN_TEST_CASE(SystemMemory, SetJmp);
}

typedef struct {
    uint32_t val1,
             val2;
} TestStruct;

static TestStruct test_struct;

TEST_SETUP(SystemMemory) {
}

TEST_TEAR_DOWN(SystemMemory) {
    test_struct = (TestStruct) { 0 };
}

TEST(SystemMemory, SetMask) {
    TestStruct * struct_p = &test_struct;
    UT_PTR_SET(system_write, mock_system_write_impl);

    system_write_Expect(&(test_struct.val1), BIT(1));
    S_SM(struct_p, val1, BIT(1), ON);

    system_write_Expect(&(test_struct.val1), BIT(2) | BIT(1));
    S_SM(struct_p, val1, BIT(2), ON);

    system_write_Expect(&(test_struct.val1), BIT(2));
    S_SM(struct_p, val1, BIT(1), OFF);

    system_write_Expect(&(test_struct.val1), BIT(3) | BIT(2));
    S_SM(struct_p, val1, BIT(3), ON);
    system_write_Expect(&(test_struct.val1), BIT(2));
    S_SM(struct_p, val1, BIT(3), OFF);
}

static void test_setjmp(jmp_buf env, int what_to_send) {
    longjmp(env, what_to_send);
}

TEST(SystemMemory, SetJmp) {
    jmp_buf env;
    int count = 0;
    int res = setjmp(env);
    if(count > 4) {
        return;
    }
    TEST_ASSERT_EQUAL(count, res);
    count ++;
    test_setjmp(env, count);
}
