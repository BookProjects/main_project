#ifndef SYSTEM_MEMORY_INTERNALS_H
#define SYSTEM_MEMORY_INTERNALS_H

#include "utils/common.h"

void * system_init(void *base_address, uint32_t type_size);
extern void (*system_write)(S_DATA *, S_DATA);
extern S_DATA (*system_read)(S_DATA *);

#endif // SYSTEM_MEMORY_INTERNALS
