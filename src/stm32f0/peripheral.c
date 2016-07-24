#include "stm32f0/peripheral.h"


PeripheralBus get_peripheral_bus(Peripheral p) {
    switch(p) {
        case TSC:
        case IO_A:
        case IO_B:
        case IO_C:
        case IO_D:
        case IO_F:
        case CRC:
        case FLITF:
        case SRAM:
        case DMA:
            return AHB;
        case CEC:
        case DAC:
        case PWR:
        case WWDG:
        case TIM2:
        case TIM3:
        case TIM6:
        case TIM14:
        case I2C1:
        case I2C2:
        case USART2:
        case SPI2:
            return APB1;
        case USART1:
        case SPI1:
        case TIM1:
        case TIM15:
        case TIM16:
        case TIM17:
        case DBG_MCU:
        case ADC:
        case SYSCFG_CMP:
            return APB2;
        default:
            // TODO: Raise / log exception
            return APB1;
    }
}
