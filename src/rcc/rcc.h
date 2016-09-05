#ifndef RCC_H
#define RCC_H

void rcc_create();
void rcc_reset();

void rcc_configure_clock(CLOCK_CONFIG config);

void rcc_reset_peripheral(PERIPH peripheral);
void rcc_enable_peripheral(PERIPH peripheral);


#endif
