#ifndef CLOCK_H
#define CLOCK_H

#include "utils/errors.h"

// Initialize the internal structure for the clock interface
void clock_create();

/*
 * initialize the system clock. Setting everything back to its default state.
 */
void clock_reset();

#endif  // CLOCK_H
