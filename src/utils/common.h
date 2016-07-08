#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdint.h>

typedef volatile uint32_t SYSTEM_DATA;
typedef volatile uint32_t S_DATA;

typedef enum {
    NIB_1 = 0,
    NIB_2 = 1
} NibbleSize;

S_DATA expand_nibble(S_DATA nibble, NibbleSize size);

#define BIT(x) (1 << x)

#endif  // COMMON_UTILS
