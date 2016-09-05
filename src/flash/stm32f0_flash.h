#ifndef STM32F0_FLASH_H
#define STM32F0_FLASH_H

#include "flash/flash.h"

typedef struct {
    S_DATA ACR;
} FlashStruct;

/* ACR - Flash Access Control Register */
#define PRFTBE BIT(4)  // Prefetch Buffer Enable
// Latency, 0 (0 < SYSCLK <= 24 MHZ) 1 (24 < SYSCLK <= 48)

#define FLASH_ADDRESS 0x40022000

#endif
