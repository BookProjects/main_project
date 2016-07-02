#ifndef STM32F0_GPIO_H
#define STM32F0_GPIO_H
/*
 * gpio.h STM32F0 GPIO Definitions
 */
#include "gpio/gpio.h"
#include "utils/system_memory.h"

// Memory Layout of GPIO as defined by the reference manual
typedef struct {
    S_DATA MODER,    // 0x00 (
           OTYPER,   // 0x04
           OSPEEDR,  // 0x08
           PUPDR,    // 0x0C
           IDR,      // 0x10
           ODR,      // 0x14
           BSRR,     // 0x18
           LCKR,     // 0x1C
           AFRL,     // 0x20
           AFRH,     // 0x24
           BRR;      // 0x28
} GPIOStruct;

#define GPIO_A_BASE_ADDRESS 0x48000000
#define GPIO_B_BASE_ADDRESS 0x48000400
#define GPIO_C_BASE_ADDRESS 0x48000800
#define GPIO_D_BASE_ADDRESS 0x48000C00
#define GPIO_F_BASE_ADDRESS 0x48001400

#endif  // STM32F0_GPIO_H
