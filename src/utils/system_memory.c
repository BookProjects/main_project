#include "utils/system_memory.h"

void * system_init(void *base_address, uint32_t type_size) {
    return base_address;
}

void system_delete(void *base_address) {
    return;
}

void system_write(S_DATA *dest, S_DATA value) {
    *dest = value;
}

S_DATA system_read(S_DATA *dest) {
    return *dest;
}
