#ifndef STM32F0_USART_H
#define STM32F0_USART_H

#include "utils/common.h"

#define USART1_ADDRESS 0x40013800
#define USART2_ADDRESS 0x40004400

typedef struct {
    S_DATA CR1,   // 0x00 (Control)
           CR2,   // 0x04 (Control)
           CR3,   // 0x08 (Control)
           BRR,   // 0x0C (Baud Rate)
           GTPR,  // 0x10 (Guard Time and Prescaler)
           RTOR,  // 0x14 (Receiver Timeout)
           RQR,   // 0x18 (Request)
           ISR,   // 0x1C (Interrupt & Status)
           ICR,   // 0x20 (Interrupt Flag Clear)
           RDR,   // 0x24 (Receive Data)
           TDR;   // 0x28 (Transmit Data)
} USARTStruct;

/* CR1 (Control Register 1) */

/* CR2 (Control Register 2) */

/* CR3 (Control Register 3) */

/* BRR (Baud Rate Register) */

/* GTPR (Guard Time and Prescaler Register) */

/* RTOR (Receiver Timeout Register) */

/* RQR (Request Register) */

/* ISR (Interrupt & Status Register) */

/* ICR (Interrupt Flag Clear Register) */

/* RDR (Receive Data Register) */

/* TDR (Transmit Data Register) */

#endif
