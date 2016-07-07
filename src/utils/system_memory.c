#include "utils/system_memory.h"

void * system_init(void *base_address, uint32_t type_size) {
    return base_address;
}

void system_delete(void *base_address) {
    return;
}

S_DATA system_read(S_DATA *dest) {
    return *dest;
}
