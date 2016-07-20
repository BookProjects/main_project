#include "utils/system_memory.h"
#include "clock/stm32f0_clock.h"
#include <stddef.h>

static RCCStruct *rcc = NULL;

void clock_create() {
    rcc = S_INIT(RCCStruct, RCC_ADDRESS);
}

void clock_reset() {
    // Fallback to default HSI Clock
    clock_set_src(HSI_CLK);
    // Disable other clocks
    clock_configure_PLL(0, 0, 0, 0);
    clock_configure_HSE(0, 0, 0);
    // Disable peripherals
    // clock_disable_peripherals(ALL_PERIPHERALS);
    clock_clear_interrupts();

    // Configure default HSI, leaving calibration alone
    S_OW(rcc, CR, 0x83, ~HSI_CAL);
}

err_t clock_set_src(CLK_SRC src) {
    return OK;
}

static CLK_SRC clock_get_src() {
    return (CLK_SRC) ((S_RD(rcc, CFGR) & SWS) >> SWS_OFFSET);
}

static CLK_SRC clock_get_pll_src() {
    return (CLK_SRC) ((S_RD(rcc, CFGR) & PLLSRC) >> PLLSRC_OFFSET);
}

static BOOL is_hsi_ready() {
    return S_RD(rcc, CR) & HSI_RDY;
}

static void set_hsi(BOOL on) {
    if(on) {
        S_OE(rcc, CR, HSI_ON);
    } else {
        S_AE(rcc, CR, ~HSI_ON);
    }
}

static BOOL is_hse_ready() {
    return S_RD(rcc, CR) & HSE_RDY;
}

static void set_hse(BOOL on) {
    if(on) {
        S_OE(rcc, CR, HSE_ON);
    } else {
        S_AE(rcc, CR, ~HSE_ON);
    }
}

err_t clock_configure_HSI(BOOL on, S_DATA trim) {
    // Configure
    S_OW(rcc, CR, trim << HSI_TRIM_OFFSET, HSI_TRIM);
    // Handle enable / disable
    BOOL currently_on = is_hsi_ready();
    if(on) {
        if(!currently_on) {
            S_OE(rcc, CR, HSI_ON);
            for(int i = 0; i < CLOCK_READY_TIMEOUT; ++i) {
                if(is_hsi_ready()) {
                    return OK;
                }
            }
        }
    } else {
        CLK_SRC clk_src = clock_get_src();
        if((clk_src == HSI_CLK) ||
                ((clk_src == PLL_CLK) &&
                 (clock_get_pll_src() == HSI_CLK))) {
            // Can't turn off current system clock
            return NOT_OK;
        }
        if(currently_on) {
            S_AE(rcc, CR, ~HSI_ON);
            for(int i = 0; i < CLOCK_READY_TIMEOUT; ++i) {
                if(!(S_RD(rcc, CR) & HSI_RDY)) {
                    return OK;
                }
            }
        }
    }
    return NOT_OK;
}

err_t clock_configure_HSE(BOOL on, S_DATA bypass_oscillator, S_DATA fallback_to_hsi) {
    // Configure
    if(fallback_to_hsi) {
        S_OE(rcc, CR, CSS_ON);
    } else {
        S_AE(rcc, CR, ~CSS_ON);
    }
    // Can only write HSE_BYP when disabled
    if(S_RD(rcc, CR) & HSE_RDY) {
        return NOT_OK;
    } else {
        if(bypass_oscillator) {
            S_OE(rcc, CR, HSE_BYP);
        } else {
            S_AE(rcc, CR, ~HSE_BYP);
        }
    }
    // Enable / Disable
    return NOT_OK;
}
err_t clock_configure_PLL(BOOL on, S_DATA multiplier, S_DATA hse_divider, S_DATA src) {
    return NOT_OK;
}


err_t clock_enable_peripheral(Peripheral peripheral) {
    switch(get_peripheral_bus(peripheral)) {
        case AHB:
            S_OE(rcc, AHBENR, get_peripheral_clock_bit(peripheral));
            break;
        case APB1:
            S_OE(rcc, APB1ENR, get_peripheral_clock_bit(peripheral));
            break;
        case APB2:
            S_OE(rcc, APB2ENR, get_peripheral_clock_bit(peripheral));
            break;
    }
    return OK;
}

err_t clock_disable_peripheral(Peripheral peripheral) {
    switch(get_peripheral_bus(peripheral)) {
        case AHB:
            S_AE(rcc, AHBENR, ~get_peripheral_clock_bit(peripheral));
            break;
        case APB1:
            S_AE(rcc, APB1ENR, ~get_peripheral_clock_bit(peripheral));
            break;
        case APB2:
            S_AE(rcc, APB2ENR, ~get_peripheral_clock_bit(peripheral));
            break;
    }
    return OK;
}

void clock_clear_interrupts() {
    S_WR(rcc, CIR, 0x0);
}
