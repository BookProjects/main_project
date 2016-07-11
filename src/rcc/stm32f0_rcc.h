#ifndef STM32F0_RCC_H
#define STM32F0_RCC_H

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
           CR2        // 0x34 (Control 2)
} RCCStruct;

/* CR (Control Register) */
// HW set, 0 means PLL unlocked, 1 is locked
#define PLLRDY   BIT(25)
// 1 = enable, hw clears when STOP or STANDBY mode
#define PLLON    BIT(24)
// Clock detector on if CSSON = 1 and HSE Oscillator is ready
#define CSSON    BIT(19)
// (HSE Crystal Oscillator Bypass), when 1 bypass oscillator in favor of ext. clk
#define HSEBYP   BIT(18)
// HSE Clock ready flag, HW sets to 0 after 6 cycles when reset HSEON
#define HSERDY   BIT(17)
// 1 = enable, hw clears when STOP or STANDBY mode
#define HSEON    BIT(16)
// HSI Clock Calibration, initialized automatically
#define HSICAL   0xFF00
// Combine with HSICAL (adjust to temp and V voltage), default = 16 -> 8MHZ +- 1%
// Trim step is about 40KHz
#define HSITRIM  0xF8
// HSI Clock ready flag, like HSERDY
#define HSIRDY   BIT(1)
// HSI Clock enable, HW sets to 1 when leaving STOP or STANDBY
#define HSION    BIT(0)


/* CFGR */
#define MCO (BIT(26) | BIT(25) | BIT(24))
#define PLLMUL 0x003C0000
#define PLLXTPRE BIT(17)
#define PLLSRC BIT(16)
#define ADCPRE BIT(14)
#define PPRE (BIT(10) | BIT(9) | BIT(8))
#define HPRE 0XF0
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

#endif
