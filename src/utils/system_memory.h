#ifndef SYSTEM_MEMORY_H
#define SYSTEM_MEMORY_H

typedef uint32_t SYSTEM_DATA;
typedef uint32_t S_DATA;


// Initialization of memory locations
#ifdef TESTING
  #include <stdlib.h>
  #define SYSTEM_INIT(type, base_address) ((type *) calloc(1, sizeof(type)))
#else
  #define SYSTEM_INIT(type, base_address) ((type *) base_address)
#endif

#define SYSTEM_WRITE(object, field, value) (object->field = value)

#define SYSTEM_READ(object, field) (object->field)

// Shortcuts
#define S_INIT SYSTEM_INIT
#define S_WR SYSTEM_WRITE
#define S_RD SYSTEM_READ

#endif // SYSTEM_MEMORY
