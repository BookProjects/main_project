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

#define BOOL uint32_t
#define bool BOOL
#define ON 1
#define OFF 0
#define true 1
#define True 1
#define TRUE 1
#define false 0
#define False 0
#define FALSE 0

#define BIT(x) (1 << x)


#endif  // COMMON_UTILS
