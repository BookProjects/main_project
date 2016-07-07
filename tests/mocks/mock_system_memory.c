#include "utils/system_memory.h"
#include <stdlib.h>

void * system_init(void *base_address, uint32_t type_size) {
    return calloc(1, type_size);
}

void system_delete(void *base_address) {
    free(base_address);
}
