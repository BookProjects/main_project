#ifndef SYSTEM_MEMORY_INTERNALS_H
#define SYSTEM_MEMORY_INTERNALS_H


typedef volatile uint32_t SYSTEM_DATA;
typedef volatile uint32_t S_DATA;

void * system_init(void *base_address, uint32_t type_size);
void system_write(S_DATA *dest, S_DATA value);
S_DATA system_read(S_DATA *src);

#endif // SYSTEM_MEMORY_INTERNALS
