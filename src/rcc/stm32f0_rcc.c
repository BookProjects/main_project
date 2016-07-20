#include "rcc/stm32f0_rcc.h"

static const S_DATA peripheral_pins[] = {
    // AHB
    [TSC] = TSC_EN,
    [IO_A] = IOA_EN,
    [IO_B] = IOB_EN,
    [IO_C] = IOC_EN,
    [IO_D] = IOD_EN,
    [IO_F] = IOF_EN,
    [CRC]  = CRC_EN,
    [FLITF] = FLITF_EN,
    [SRAM] = SRAM_EN,
    [DMA] = DMA_EN,
    // APB1
    [CEC] = CEC_EN,
    [DAC] = DAC_EN,
    [PWR] = PWR_EN,
    [WWDG] = WWDG_EN,
    [TIM2] = TIM2_EN,
    [TIM3] = TIM3_EN,
    [TIM6] = TIM6_EN,
    [TIM14] = TIM14_EN,
    [I2C1] = I2C1_EN,
    [I2C2] = I2C2_EN,
    [USART2] = USART2_EN,
    [SPI2] = SPI2_EN,
    // APB2
    [USART1] = USART1_EN,
    [SPI1] = SPI1_EN,
    [TIM1] = TIM1_EN,
    [TIM15] = TIM15_EN,
    [TIM16] = TIM16_EN,
    [TIM17] = TIM17_EN,
    [DBG_MCU] = DBG_MCU_EN,
    [ADC] = ADC_EN,
    [SYSCFG_CMP] = SYSCFG_CMP_EN
};

S_DATA get_peripheral_clock_bit(Peripheral p) {
    return peripheral_pins[p];
}
