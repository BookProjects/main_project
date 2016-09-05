#ifndef STM32F0_INITIALIZATION_H
#define STM32F0_INITIALIZATION_H

void processor_initialization(void) {
    // Setup HSI as clock, turn off HSE / CSS, PLL
    // Reset all prescalars and options for PLL
    // Reset peripheral clock sources
    // Disable CLK ready interrupts
    clock_create();

    // Configure SYSCLK, AHB/APBx, return frequency
    ClockConfig clock_config;
    S_DATA frequency = clock_configure(clock_config);

    // Initialize flash
    flash_create(frequency);
}

#endif  // STM32F0_INITIALIZATION_H
