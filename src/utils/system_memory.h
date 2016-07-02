#ifdef TESTING
  #include <stdlib.h>
  #define SYSTEM_INIT(type, base_address) ((type *) calloc(1, sizeof(type)))
#else
  #define SYSTEM_INIT(type, base_address) ((type *) base_address)
#endif
#define S_INIT SYSTEM_INIT
