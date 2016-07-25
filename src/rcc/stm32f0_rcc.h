#ifndef STM32F0_RCC_H
#define STM32F0_RCC_H

#include "utils/common.h"
#include "processor/stm32f0/peripheral.h"

S_DATA get_peripheral_rcc_bit(Peripheral p);

#define RCC_ADDRESS 0x40021000

typedef struct {
    S_DATA CR,        // 0x00 (Control)
           CFGR,      // 0x04 (Configuration)
           CIR,       // 0x08 (Interrupt)
           APB2RSTR,  // 0x0C (APB2 Peripheral Reset)
           APB1RSTR,  // 0x10 (APB1 Peripheral Reset)
           AHBENR,    // 0x14 (AHB Peripheral Clock Enable)
           APB2ENR,   // 0x18 (APB2 Peripheral Clock Enable)
           APB1ENR,   // 0x1C (APB1 Peripheral Clock Enable)
           BDCR,      // 0x20 (Backup Domain Control)
           CSR,       // 0x24 (Control / Status)
           AHBRSTR,   // 0x28 (AHB Peripheral Reset)
           CFGR2,     // 0x2C (Clock Configuration 2)
           CFGR3,     // 0x30 (Clock Configuration 3)
           CR2;       // 0x34 (Control 2)
} RCCStruct;

/* CR (Control Register) */
// HW set, 0 means PLL unlocked, 1 is locked
#define PLL_RDY   BIT(25)
// 1 = enable, hw clears when STOP or STANDBY mode
#define PLL_ON    BIT(24)
// Clock detector on if CSSON = 1 and HSE Oscillator is ready
#define CSS_ON    BIT(19)
// (HSE Crystal Oscillator Bypass), when 1 bypass oscillator in favor of ext. clk
#define HSE_BYP   BIT(18)
// HSE Clock ready flag, HW sets to 0 after 6 cycles when reset HSEON
#define HSE_RDY   BIT(17)
// 1 = enable, hw clears when STOP or STANDBY mode
#define HSE_ON    BIT(16)
// HSI Clock Calibration, initialized automatically
#define HSI_CAL   0xFF00
// Combine with HSICAL (adjust to temp and V voltage), default = 16 -> 8MHZ +- 1%
// Trim step is about 40KHz
#define HSI_TRIM  0xF8
#define HSI_TRIM_OFFSET 3
// HSI Clock ready flag, like HSERDY
#define HSI_RDY   BIT(1)
// HSI Clock enable, HW sets to 1 when leaving STOP or STANDBY
#define HSI_ON    BIT(0)


/* CFGR (Clock Configuration Register) */
#define MCO (BIT(26) | BIT(25) | BIT(24))
#define PLLMUL 0x003C0000
#define PLLXTPRE BIT(17)
#define PLLSRC_OFFSET 16
#define PLLSRC BIT(PLLSRC_OFFSET)
#define ADCPRE BIT(14)
#define PPRE (BIT(10) | BIT(9) | BIT(8))
#define HPRE 0XF0
#define SWS_OFFSET 2
#define SWS 0x0C
#define SW 0x03

typedef enum {
    MCO_DISABLED      = 0,
    HSI_14_OUT        = 3,
    SYS_CLK_OUT       = 4,
    HSI_CLK_OUT       = 5,
    HSE_CLK_OUT       = 6,
    PLL_CLK_DIV_2_OUT = 7
} MICRO_CLK_OUT;

typedef enum {
    PLL_2  = 0x0,
    PLL_3  = 0x1,
    PLL_4  = 0x2,
    PLL_5  = 0x3,
    PLL_6  = 0x4,
    PLL_7  = 0x5,
    PLL_8  = 0x6,
    PLL_9  = 0x7,
    PLL_10 = 0x8,
    PLL_11 = 0x9,
    PLL_12 = 0xA,
    PLL_13 = 0xB,
    PLL_15 = 0xC,
    PLL_16 = 0xD
} PLL_MULT_FACT;

typedef enum {
    PCLK_SCL_1  = 0,
    PCLK_SCL_2  = 4,
    PCLK_SCL_4  = 5,
    PCLK_SCL_8  = 6,
    PCLK_SCL_16 = 7
} PCLK_PRESCALER;

typedef enum {
    HCLK_SCL_1   = 0,
    HCLK_SCL_2   = 0x08,
    HCLK_SCL_4   = 0x09,
    HCLK_SCL_8   = 0x0A,
    HCLK_SCL_16  = 0x0B,
    HCLK_SCL_64  = 0x0C,
    HCLK_SCL_128 = 0x0D,
    HCLK_SCL_256 = 0x0E,
    HCLK_SCL_512 = 0x0F
} HCLK_PRESCALER;

typedef enum {
    HSI = 0,
    HSE = 1,
    PLL = 2
} CLK_SW;


/* CIR (Clock Interrupt Register) */
#define CSSC       BIT(23)  // Clock Security System Interrupt Clear, clears CSSF
// RDYC -> Ready Interrupt Clear, clears RDYF Flag
#define HSI14_RDYC BIT(21)
#define PLL_RDYC   BIT(20)
#define HSE_RDYC   BIT(19)
#define HSI_RDYC   BIT(18)
#define LSE_RDYC   BIT(17)
#define LSI_RDYC   BIT(16)

// RDYIE -> Ready Interrupt Enable
#define HSI14_RDYIE BIT(13)
#define PLL_RDYIE   BIT(12)
#define HSE_RDYIE   BIT(11)
#define HSI_RDYIE   BIT(10)
#define LSE_RDYIE   BIT(9)
#define LSI_RDYIE   BIT(8)
// Clock Security System Interrupt Flag, interrupt caused by HSE Clock Failure
#define CSSF        BIT(7)

// RDYF -> Ready Interrupt Flag, set when x becomes stable and xON bit set in
// CFGR2 to enable clock
#define HSI14_RDYF  BIT(5)
#define PLL_RDYF    BIT(4)
#define HSE_RDYF    BIT(3)
#define HSI_RDYF    BIT(2)
#define LSE_RDYF    BIT(1)
#define LSI_RDYF    BIT(0)


/* APB2RSTR (APB2 Peripheral Reset Register) */
#define DBG_MCU_RST     BIT(22)
#define TIM17_RST       BIT(18)
#define TIM16_RST       BIT(17)
#define TIM15_RST       BIT(16)
#define USART1_RST      BIT(14)
#define SPI1_RST        BIT(12)
#define TIM1_RST        BIT(11)
#define ADC_RST         BIT(9)
#define SYSCFG_CMP_RST  BIT(0)


/* APB1RSTR (APB1 Peripheral Reset Register) */
#define CEC_RST    BIT(30)
#define DAC_RST    BIT(29)
#define PWR_RST    BIT(28)
#define I2C2_RST   BIT(22)
#define I2C1_RST   BIT(21)
#define USART2_RST BIT(17)
#define SPI2_RST   BIT(14)
#define WWDG_RST   BIT(11)
#define TIM14_RST  BIT(8)
#define TIM6_RST   BIT(4)
#define TIM3_RST   BIT(1)
#define TIM2_RST   BIT(0)


/* AHBENR (AHB Peripheral Clock Enable Register) */
#define TSC_EN   BIT(24)
#define IOF_EN   BIT(22)
#define IOD_EN   BIT(20)
#define IOC_EN   BIT(19)
#define IOB_EN   BIT(18)
#define IOA_EN   BIT(17)
#define CRC_EN   BIT(6)
#define FLITF_EN BIT(4)  // FLITF clock enable during SLEEP mode
#define SRAM_EN  BIT(2)  // SRAM clock enable during SLEEP Mode
#define DMA_EN   BIT(0)


/* APB2ENR (APB2 Peripheral Clock Enable Register) */
#define DBG_MCU_EN     BIT(22)
#define TIM17_EN       BIT(18)
#define TIM16_EN       BIT(17)
#define TIM15_EN       BIT(16)
#define USART1_EN      BIT(14)
#define SPI1_EN        BIT(12)
#define TIM1_EN        BIT(11)
#define ADC_EN         BIT(9)
#define SYSCFG_CMP_EN  BIT(0)


/* APB1ENR (APB1 Peripheral Clock Enable Register) */
#define CEC_EN    BIT(30)
#define DAC_EN    BIT(29)
#define PWR_EN    BIT(28)
#define I2C2_EN   BIT(22)
#define I2C1_EN   BIT(21)
#define USART2_EN BIT(17)
#define SPI2_EN   BIT(14)
#define WWDG_EN   BIT(11)
#define TIM14_EN  BIT(8)
#define TIM6_EN   BIT(4)
#define TIM3_EN   BIT(1)
#define TIM2_EN   BIT(0)


/* BDCR (Backup Domain Control Register)
 *
 * Backup domain reset is only during:
 * 1) SW reset by setting BDRST bit in BDCR
 * 2) V_DD or V_BAT power on if both were off
 *
 * LSEON, LSEBYP, RTCSEL, RTCEN are in backup domain. After reset, these are
 * write-protected. DBP bit in PWR_CR must be set first.
 */
#define BDRST   BIT(16)  // Backup domain software reset
#define RTC_EN  BIT(15)
#define RTC_SEL (BIT(9) | BIT(8))
typedef enum {
    NO_RTC         = 0,
    LSE_RTC        = 1,
    LSI_RTC        = 2,
    HSE_DIV_32_RTC = 3
} RTC_SRC;
#define LSE_DRV (BIT(4) | BIT(3))
typedef enum {
    LOW_DRV_CAP      = 0,
    MED_LOW_DRV_CAP  = 1,
    MED_HIGH_DRV_CAP = 2,
    HIGH_DRV_CAP     = 3
} LSE_DRV_CAP;
#define LSE_BYP BIT(2)
#define LSE_RDY BIT(1)
#define LSE_ON  BIT(0)


/* CSR (Control / Status Register)
 *
 * RSTF's are set by hardware when resets occur
 */
#define LPWR_RSTF  BIT(31)
#define WWDG_RSTF  BIT(30)
#define IWWDG_RSTF BIT(29)
#define SFT_RSTF   BIT(28)
#define POR_RSTF   BIT(27)
#define PIN_RSTF   BIT(26)
#define OBL_RSTF   BIT(25)
#define RMVF       BIT(24)  // Clear the reset flags
#define LSI_RDY    BIT(1)
#define LSI_ON     BIT(0)

/* AHBRSTR (AHB Peripheral Reset Register) */
#define TSC_RST   BIT(24)
#define IOF_RST   BIT(22)
#define IOD_RST   BIT(20)
#define IOC_RST   BIT(19)
#define IOB_RST   BIT(18)
#define IOA_RST   BIT(17)


/* CFGR2 (Clock Configuration Register 2)
 *
 * Only writable while PLL is disabled.
 * Bit 0 is the same as bit 17 in CFGR, so any writes there show up here.
 * HSE input = PLL / (PREDIV + 1)
 *
 * PREDIV = bits[3:0]
 */


/* CFGR3 (Clock Configuration Register 3) */
#define ADC_SW BIT(8)  // 0 = HSI14 as ADC kernel clock, 1 = PCLK / 2 or 4
#define CEC_SW BIT(6)  // 0 = HSI / 244, 1 = LSE
#define I2C1_SW BIT(4)  // 0 = HSI, 1 = SYSCLK
#define USART1_SW (BIT(1) | BIT(0))
typedef enum {
    USART_PCLK   = 0,
    USART_SYSCLK = 1,
    USART_LSE    = 2,
    USART_HSI    = 3
} USART_CLK_SRC;


/* CR2 (Clock Control Register 2)
 *
 * Very similar to the Bottom half of CR, s/HSI/HSI14
 *
 */
// HSI14 Clock Calibration, initialized automatically
#define HSI14_CAL   0xFF00
// Combine with HSI14CAL (adjust to temp and V voltage), default = 16 -> 8MHZ +- 1%
// Trim step is about 50KHz (different from 40khz of HSI
#define HSI14_TRIM  0xF8
// HSI14 Clock request from ADC Disable (when set, prevents ADC from enabling HSI14)
#define HSI14_DIS   BIT(2)
// HSI14 Clock ready flag, like HSERDY
#define HSI14_RDY   BIT(1)
// HSI14 Clock enable, HW sets to 1 when leaving STOP or STANDBY
#define HSI14_ON    BIT(0)


#endif
