#include "utils/system_memory.h"

void system_write(S_DATA *dest, S_DATA value) {
    *dest = value;
}

S_DATA system_read(S_DATA *dest) {
    return *dest;
}
