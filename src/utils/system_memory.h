#ifndef SYSTEM_MEMORY_H
#define SYSTEM_MEMORY_H

#include <stdint.h>

typedef volatile uint32_t SYSTEM_DATA;
typedef volatile uint32_t S_DATA;


// Initialization of memory locations
#define SYSTEM_INIT(type, base_address) ((type *) system_init((void *)base_address, sizeof(type)))
void * system_init(void *base_address, uint32_t type_size);

#define SYSTEM_WRITE(object, field, value) (system_write(&(object->field), value))
void system_write(S_DATA *dest, S_DATA value);

#define SYSTEM_READ(object, field) (system_read(&(object->field)))
S_DATA system_read(S_DATA *src);

// Shortcuts
#define S_INIT SYSTEM_INIT
#define S_WR SYSTEM_WRITE
#define S_RD SYSTEM_READ

#endif // SYSTEM_MEMORY
