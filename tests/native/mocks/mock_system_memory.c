#include "utils/system_memory.h"
//#include "mocks/mock_system_memory_internals.h"
#include <stdlib.h>

void system_delete(void *base_address) {
}

static void system_write_impl(S_DATA *dest, S_DATA value) {
    *dest = value;
}
void (*system_write)(S_DATA *, S_DATA) = system_write_impl;

static S_DATA system_read_impl(S_DATA *src) {
    return *src;
}
S_DATA (*system_read)(S_DATA *) = system_read_impl;
