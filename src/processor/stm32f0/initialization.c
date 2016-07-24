#include "processor/initialization.h"

#include "clock/clock.h"

void processor_initialization(void) {
    // Initialize clock interface
    clock_create();
    // Setup HSI as clock, turn off HSE / CSS, PLL
    // Reset all prescalars and options for PLL
    // Reset peripheral clock sources
    // Disable CLK ready interrupts
    clock_reset();

    // Configure SYSCLK, AHB/APBx, return frequency
    // ClockConfig clock_config;
    // S_DATA frequency = clock_configure(clock_config);

    // Initialize flash
    // flash_create(frequency);
}
