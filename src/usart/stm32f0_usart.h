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
#define EOBIE
#define RTOIE
#define DEAT
#define DEDT
#define OVER8
#define CMIE
#define MME
#define M
#define WAKE
#define PCE
#define PS
#define PEIE
#define TXEIE
#define TCIE
#define RXNEIE
#define IDLEIE
#define TE
#define RE
#define UESM
#define UE


/* CR2 (Control Register 2) */
#define ADD
#define RTOEN
#define ABRMOD
#define ABREN
#define MSBFIRST
#define DATAINV
#define TXINV
#define RXINV
#define SWAP
#define LINEN
#define STOP
#define CLKEN
#define CPOL
#define CPHA
#define LBCL
#define SSM
#define LBDIE
#define LBDL
#define ADDM7

/* CR3 (Control Register 3) */
#define WUFIE
#define WUS
#define SCARCNT
#define DEP
#define DEM
#define DDRE
#define OVRDIS
#define ONEBIT
#define CTSIE
#define CTSE
#define RTSE
#define DMAT
#define DMAR
#define SCEN
#define NACK
#define HDSEL
#define IRLP
#define IREN
#define EIE

/* BRR (Baud Rate Register)
 *
 * [15:4] = USARTDIV[15:4]
 * OVER8 = 0 | [3:0] = USARTDIV[3:0]
 * OVER8 = 1 | [2:0] = USARTDIV[3:0] shifted right by 1 bit, [3] must be kept
 * cleared.
 */

/* GTPR (Guard Time and Prescaler Register) */
#define GT_OFFSET 8


/* RTOR (Receiver Timeout Register) */
#define BLEN_OFFSET 24

/* RQR (Request Register) */
#define TXFRQ
#define RXFRQ
#define MMRQ
#define SBKRQ
#define ABRRQ

/* ISR (Interrupt & Status Register) */
#define REACK
#define TEACK
#define WUF
#define RWU
#define SBKF
#define CMF
#define BUSY
#define ABRF
#define ABRE
#define EOBF
#define RTOF
#define CTS
#define CTSIF
#define LBDF
#define TXE
#define TC
#define RXNE
#define IDLE
#define ORE
#define NF
#define FE
#define PE


/* ICR (Interrupt Flag Clear Register) */
#define WUCF
#define CMCF
#define EOBCF
#define RTOCF
#define CTSCF
#define LBDCF
#define TCCF
#define IDLECF
#define ORECF
#define NCF
#define FECF
#define PECF

/* RDR (Receive Data Register) */

/* TDR (Transmit Data Register) */

#endif
