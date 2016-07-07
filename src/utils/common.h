#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdint.h>

typedef volatile uint32_t SYSTEM_DATA;
typedef volatile uint32_t S_DATA;

S_DATA expand_nibble(S_DATA nibble, uint8_t nibble_width, uint8_t num_expansions);

#endif  // COMMON_UTILS
