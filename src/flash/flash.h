#ifndef FLASH_H
#define FLASH_H

#include "utils/common.h"
#include "utils/system_memory.h"
#include <stddef.h>

void flash_create();
void flash_reset();

void flash_configure(BOOL enable_prefetch, BOOL sysclk_more_than_48_mhz);

#endif
