#ifndef SYSTEM_MEMORY_H
#define SYSTEM_MEMORY_H

#include <stdint.h>
#include "utils/system_memory_internals.h"

// Initialization of memory locations
#define SYSTEM_INIT(type, base_address) ((type *) system_init((void *)base_address, sizeof(type)))

#define SYSTEM_DELETE(base_address) (system_delete((void *)base_address))
void system_delete(void *base_address);

#define SYSTEM_WRITE(object, field, value) (system_write(&(object->field), value))

#define SYSTEM_READ(object, field) (system_read(&(object->field)))

// Shortcuts
#define S_INIT SYSTEM_INIT
#define S_DEL SYSTEM_DELETE
#define S_WR SYSTEM_WRITE
#define S_RD SYSTEM_READ

#endif // SYSTEM_MEMORY
