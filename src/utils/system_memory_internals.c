#include "utils/system_memory.h"

static void system_write_impl(S_DATA *dest, S_DATA value) {
    *dest = value;
}

void (*system_write)(S_DATA *, S_DATA) = system_write_impl;

