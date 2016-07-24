#ifndef STM32F0_PERIPHERAL_H
#define STM32F0_PERIPHERAL_H

typedef enum {
   // AHB 
   TSC,
   IO_A,
   IO_B,
   IO_C,
   IO_D,
   IO_F,
   CRC,
   FLITF,
   SRAM,
   DMA,

   // APB1
   CEC,
   DAC,
   PWR,
   WWDG,
   TIM2,
   TIM3,
   TIM6,
   TIM14,
   I2C1,
   I2C2,
   USART2,
   SPI2,

   // APB2
   USART1,
   SPI1,
   TIM1,
   TIM15,
   TIM16,
   TIM17,
   DBG_MCU,
   ADC,
   SYSCFG_CMP
} Peripheral;

typedef enum {
    AHB,
    APB1,
    APB2
} PeripheralBus;

PeripheralBus get_peripheral_bus(Peripheral p);

#endif  // STM32F0_PERIPHERAL
