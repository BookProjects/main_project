#ifndef STM32F0_GPIO_H
#define STM32F0_GPIO_H
/*
 * gpio.h STM32F0 GPIO Definitions
 */
#include "gpio/gpio.h"
#include "utils/system_memory.h"

// Memory Layout of GPIO as defined by the reference manual
typedef struct {
    S_DATA MODER,    // 0x00 (mode, [00 input, 01 output, 10 AF, 11 ALG])
           OTYPER,   // 0x04 (type, [0 push-pull, 1 open-drain])
           OSPEEDR,  // 0x08 (speed, [00 2MHZ, 01 10MHZ, 11 50MHZ])
           PUPDR,    // 0x0C (pull up/dn, [00 none, 01 up, 10 dn, 11 <res>])
           IDR,      // 0x10 (input data register)
           ODR,      // 0x14 (output data register)
           BSRR,     // 0x18 (bit set reset, 31:16 reset, 15:0 set)
           LCKR,     // 0x1C (lock register)
           AFRL,     // 0x20 (alternate function low register)
           AFRH,     // 0x24 (alternate function high register)
           BRR;      // 0x28 (bit reset register)
} GPIOStruct;

#define GPIO_A_BASE_ADDRESS 0x48000000
#define GPIO_B_BASE_ADDRESS 0x48000400
#define GPIO_C_BASE_ADDRESS 0x48000800
#define GPIO_D_BASE_ADDRESS 0x48000C00
#define GPIO_F_BASE_ADDRESS 0x48001400

#endif  // STM32F0_GPIO_H
