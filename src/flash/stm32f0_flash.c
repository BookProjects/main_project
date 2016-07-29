#include "flash/stm32f0_flash.h"

static FlashStruct *flash = NULL;

void flash_create() {
    flash = S_INIT(FlashStruct, FLASH_ADDRESS);
}

void flash_configure(BOOL enable_prefetch, BOOL sysclk_more_than_48_mhz) {
    S_DATA configure_val = 0x0;
    configure_val |= enable_prefetch ? PRFTBE : 0;
    configure_val |= sysclk_more_than_48_mhz ? 1 : 0;
    S_WR(flash, ACR, configure_val);
}
