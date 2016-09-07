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
#define EOBIE BIT(27)
#define RTOIE BIT(26)
#define DEAT 0x3E00000
#define DEAT_OFFSET 21
#define DEDT 0x01F0000
#define DEDT_OFFSET 16
#define OVER8 BIT(15)
#define CMIE BIT(14)
#define MME BIT(13)
#define M BIT(12)
#define WAKE BIT(11)
#define PCE BIT(10)
#define PS BIT(9)
#define PEIE BIT(8)
#define TXEIE BIT(7)
#define TCIE BIT(6)
#define RXNEIE BIT(5)
#define IDLEIE BIT(4)
#define TE BIT(3)
#define RE BIT(2)
#define UESM BIT(1)
#define UE BIT(0)


/* CR2 (Control Register 2) */
#define ADD 0xFF000000
#define ADD_OFFSET 24
#define RTOEN BIT(23)
#define ABRMOD (BIT(22) | BIT(21))
#define ABRMOD_OFFSET 21
#define ABREN BIT(20)
#define MSBFIRST BIT(19)
#define DATAINV BIT(18)
#define TXINV BIT(17)
#define RXINV BIT(16)
#define SWAP BIT(15)
#define LINEN BIT(14)
#define STOP (BIT(13) | BIT(12))
#define STOP_OFFSET 12
#define CLKEN BIT(11)
#define CPOL BIT(11)
#define CPHA BIT(10)
#define LBCL BIT(9)
#define SSM BIT(7)
#define LBDIE BIT(6)
#define LBDL BIT(5)
#define ADDM7 BIT(4)

/* CR3 (Control Register 3) */
#define WUFIE BIT(22)
#define WUS (BIT(21) | BIT(20)
#define WUS_OFFSET 20
#define SCARCNT (BIT(19) | BIT(18) | BIT(17))
#define SCARCNT_OFFSET 17
#define DEP BIT(15)
#define DEM BIT(14)
#define DDRE BIT(13)
#define OVRDIS BIT(12)
#define ONEBIT BIT(11)
#define CTSIE BIT(10)
#define CTSE BIT(9)
#define RTSE BIT(8)
#define DMAT BIT(7)
#define DMAR BIT(6)
#define SCEN BIT(5)
#define NACK BIT(4)
#define HDSEL BIT(3)
#define IRLP BIT(2)
#define IREN BIT(1)
#define EIE BIT(0)

/* BRR (Baud Rate Register)
 *
 * [15:4] = USARTDIV[15:4]
 * OVER8 = 0 | [3:0] = USARTDIV[3:0]
 * OVER8 = 1 | [2:0] = USARTDIV[3:0] shifted right by 1 bit, [3] must be kept
 * cleared.
 */

/* GTPR (Guard Time and Prescaler Register) */
#define GT 0xFF00
#define GT_OFFSET 8


/* RTOR (Receiver Timeout Register) */
#define BLEN 0xFF000000
#define BLEN_OFFSET 24

/* RQR (Request Register) */
#define TXFRQ BIT(4)
#define RXFRQ BIT(3)
#define MMRQ BIT(2)
#define SBKRQ BIT(1)
#define ABRRQ BIT(0)

/* ISR (Interrupt & Status Register) */
#define REACK BIT(22)
#define TEACK BIT(21)
#define WUF BIT(20)
#define RWU BIT(19)
#define SBKF BIT(18)
#define CMF BIT(17)
#define BUSY BIT(16)
#define ABRF BIT(15)
#define ABRE BIT(14)
#define EOBF BIT(12)
#define RTOF BIT(11)
#define CTS BIT(10)
#define CTSIF BIT(9)
#define LBDF BIT(8)
#define TXE BIT(7)
#define TC BIT(6)
#define RXNE BIT(5)
#define IDLE BIT(4)
#define ORE BIT(3)
#define NF BIT(2)
#define FE BIT(1)
#define PE BIT(0)


/* ICR (Interrupt Flag Clear Register) */
#define WUCF BIT(20)
#define CMCF BIT(17)
#define EOBCF BIT(12)
#define RTOCF BIT(11)
#define CTSCF BIT(9)
#define LBDCF BIT(8)
#define TCCF BIT(6)
#define IDLECF BIT(4)
#define ORECF BIT(3)
#define NCF BIT(2)
#define FECF BIT(1)
#define PECF BIT(0)

/* RDR (Receive Data Register) */

/* TDR (Transmit Data Register) */

#endif
