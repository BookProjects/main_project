#include "utils/system_memory.h"
#include "clock/stm32f0_clock.h"
#include <stddef.h>

// Private helper functions
static CLK_SRC clock_get_src();
static CLK_SRC clock_get_pll_src();

static BOOL is_src_ready(CLK_SRC src);

static BOOL is_hsi_ready();
static BOOL is_hse_ready();
static BOOL is_pll_ready();

static BOOL is_hsi_src();
static BOOL is_hse_src();

static void set_hsi(BOOL on);
static void set_hse(BOOL on);

static err_t power_clock(
        BOOL on,
        BOOL(*is_clk_ready)(void),
        BOOL(*is_clk_src)(void),
        void (*set_clk)(BOOL));

// Private Variables
static RCCStruct *rcc = NULL;


void clock_create() {
    rcc = S_INIT(RCCStruct, RCC_ADDRESS);
}

void clock_reset() {
    // Fallback to default HSI Clock
    clock_configure_HSI(ON, 0);
    clock_set_src(HSI_CLK);
    // Disable other clocks
    clock_configure_PLL(0, 0, 0, 0);
    clock_configure_HSE(0, 0, 0);
    // Disable peripherals
    // clock_disable_peripherals(ALL_PERIPHERALS);
    clock_clear_interrupts();
}

err_t clock_set_src(CLK_SRC src) {
    if(is_src_ready(src)) {
        S_OW(rcc, CR, src, SW);
        for(int i = 0; i < CLOCK_READY_TIMEOUT; ++i) {
            if(clock_get_src() == src) {
                return OK;
            }
        }
    }
    return NOT_OK;
}

err_t clock_configure_HSI(BOOL on, S_DATA trim) {
    // Configure
    S_OW(rcc, CR, trim << HSI_TRIM_OFFSET, HSI_TRIM);
    // Turn on / off
    return power_clock(on, is_hsi_ready, is_hsi_src, set_hsi);
}

err_t clock_configure_HSE(BOOL on, S_DATA bypass_oscillator, S_DATA fallback_to_hsi) {
    // Configure
    S_SM(rcc, CR, CSS_ON, fallback_to_hsi);

    // Can only write HSE_BYP when disabled
    if(is_hse_src()) {
        return NOT_OK;
    }
    // Turn it off, so we can configure it
    if(power_clock(OFF, is_hse_ready, is_hse_src, set_hse) == NOT_OK) {
        return NOT_OK;
    }
    S_SM(rcc, CR, HSE_BYP, bypass_oscillator);

    // Enable / Disable
    return power_clock(on, is_hse_ready, is_hse_src, set_hse);
}

err_t clock_configure_PLL(BOOL on, S_DATA multiplier, S_DATA hse_divider, S_DATA src) {
    return NOT_OK;
}


// Peripheral Clocks
err_t clock_power_peripheral(Peripheral peripheral, BOOL on) {
    switch(get_peripheral_bus(peripheral)) {
        case AHB:
            S_SM(rcc, AHBENR, get_peripheral_rcc_bit(peripheral), on);
            break;
        case APB1:
            S_SM(rcc, APB1ENR, get_peripheral_rcc_bit(peripheral), on);
            break;
        case APB2:
            S_SM(rcc, APB2ENR, get_peripheral_rcc_bit(peripheral), on);
            break;
    }
    return OK;
}

err_t clock_disable_peripheral(Peripheral peripheral) {
    switch(get_peripheral_bus(peripheral)) {
        case AHB:
            S_AE(rcc, AHBENR, ~get_peripheral_rcc_bit(peripheral));
            break;
        case APB1:
            S_AE(rcc, APB1ENR, ~get_peripheral_rcc_bit(peripheral));
            break;
        case APB2:
            S_AE(rcc, APB2ENR, ~get_peripheral_rcc_bit(peripheral));
            break;
    }
    return OK;
}

void clock_clear_interrupts() {
    S_WR(rcc, CIR, 0x0);
}


// Private helper function implementations
static CLK_SRC clock_get_src() {
    return (CLK_SRC) ((S_RD(rcc, CFGR) & SWS) >> SWS_OFFSET);
}

static CLK_SRC clock_get_pll_src() {
    return (CLK_SRC) ((S_RD(rcc, CFGR) & PLLSRC) >> PLLSRC_OFFSET);
}


static BOOL is_src_ready(CLK_SRC src) {
    switch(src) {
        case HSI_CLK:
            return is_hsi_ready();
        case HSE_CLK:
            return is_hse_ready();
        case PLL_CLK:
            return is_pll_ready();
        default:
            return OFF;
    }
}

static BOOL is_hsi_ready() {
    return S_RD(rcc, CR) & HSI_RDY;
}

static BOOL is_hse_ready() {
    return S_RD(rcc, CR) & HSE_RDY;
}

static BOOL is_pll_ready() {
    return S_RD(rcc, CR) & PLL_RDY;
}


static BOOL is_hsi_src() {
    CLK_SRC clk_src = clock_get_src();
    return (clk_src == HSI_CLK) ||
           ((clk_src == PLL_CLK) &&
            (clock_get_pll_src() == HSI_CLK));
}

static BOOL is_hse_src() {
    CLK_SRC clk_src = clock_get_src();
    return (clk_src == HSE_CLK) ||
           ((clk_src == PLL_CLK) &&
            (clock_get_pll_src() == HSE_CLK));
}


static void set_hsi(BOOL on) {
    S_SM(rcc, CR, HSI_ON, on);
}

static void set_hse(BOOL on) {
    S_SM(rcc, CR, HSE_ON, on);
}


static err_t power_clock(
        BOOL on,
        BOOL(*is_clk_ready)(void),
        BOOL(*is_clk_src)(void),
        void (*set_clk)(BOOL)) {
    BOOL currently_on = is_clk_ready();
    if(on) {
        if(!currently_on) {
            set_clk(ON);
            for(int i = 0; i < CLOCK_READY_TIMEOUT; ++i) {
                if(is_clk_ready()) {
                    return OK;
                }
            }
        } else {
            return OK;
        }
    } else {
        if(is_clk_src()) {
            // Can't turn off current system clock
            return NOT_OK;
        }
        if(currently_on) {
            set_clk(OFF);
            for(int i = 0; i < CLOCK_READY_TIMEOUT; ++i) {
                if(!is_clk_ready) {
                    return OK;
                }
            }
        } else {
            return OK;
        }
    }
    return NOT_OK;
}
