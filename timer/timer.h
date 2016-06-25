#ifndef TIMER_H
#define TIMER_H
/*
 * timer.h - STM32F0 Timer Definitions
 */

/*
 * Timer data structure for stm32f0 timers. There are many timers with varying
 * functionalities:
 * [
 *   1      - Advanced-control timer
 *   2, 3   - General-purpose timer
 *   14     - General-purpose timer
 *   15     - General-purpose timer
 *   16, 17 - General-purpose timer
 *   6      - Basic timer
 * ]
 */
typedef struct {
    uint32_t CR1,    // 0x00
             /*
              * Timer   | 9 : 8 | 7    | 6 : 5 | 4   | 3   | 2   | 1    | 0
              * --------------------------------------------------------------
              * 1, 2, 3 | CKD   | ARPE | CMS   | DIR | OPM | URS | UDIS | CEN
              * 14      | CKD   | ARPE |       |     |     | URS | UDIS | CEN
              * 14      | CKD   | ARPE |       |     | OPM | URS | UDIS | CEN
              */

             CR2,    // 0x04 - unused in [14]
             /*
              * Timer | 14   | 13    | 12   | 11    | 10   | 9     | 8    | 7    | 6 : 4 |  3   | 2    | 1     | 0
              * ---------------------------------------------------------------------------------------------------
              *  1    | OIS4 | OIS3N | OIS3 | OIS2N | OIS2 | OIS1N | OIS1 | TI1S | MMS   | CCDS | CCUS | <RES> | CCPC
              *  2, 3 |                                                   | TI1S | MMS   | CCDS |
              *  15   |      |       |      |       | OIS2 | OIS1N | OIS1 |      | MMS   | CCDS | CCUS |       | CCPC
              */

             SMCR,   // 0x08 - unused in [14, 16, 17, 6]
             /*
              * Timer    | 15  | 14   | 13 : 12 | 11 : 8 | 7    | 6 : 4 |  3    | 2 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3 | ETP | ECE  | ETPS    | ETF    | MSM  | TS    | <RES> | SMS
              *  15      |     |      |         |        | MSM  | TS    | <RES> | SMS
              */

             DIER,   // 0x0C
             /*
              * Timer | 14   | 13    | 12    | 11    | 10    | 9     | 8   | 7       | 6   | 5     |  4    |  3    | 2     | 1     | 0
              * -----------------------------------------------------------------------------------------------------------------------
              *  1    | TDE  | COMDE | CC4DE | CC3DE | CC2DE | CC1DE | UDE | BIE     | TIE | COMIE | CC4IE | CC3IE | CC2IE | CC1IE | UIE
              *  2, 3 | TDE  |       | CC4DE | CC3DE | CC2DE | CC1DE | UDE |         | TIE |       | CC4IE | CC3IE | CC2IE | CC1IE | UIE
              *  14   |      |       |       |       |       |       |     |         |     |       |       |       |       | CC1IE | UIE
              *  15   | TDE  |       |       |       | CC2DE | CC1DE | UDE | BIE     | TIE | COMIE |       |       | CC2IE | CC1IE | UIE
              */

             SR,     // 0x10
             /*
              * Timer |                12    | 11    | 10    | 9     | 8     | 7     | 6   | 5     |  4    |  3    | 2     | 1     | 0
              * -----------------------------------------------------------------------------------------------------------------------
              *  1    |                CC4OF | CC3OF | CC2OF | CC1OF | <RES> | BIF   | TIF | COMIF | CC4IF | CC3IF | CC2IF | CC1IF | UIF
              *  2, 3 |                CC4OF | CC3OF | CC2OF | CC1OF | <RES> |       | TIF | COMIF | CC4IF | CC3IF | CC2IF | CC1IF | UIF
              *  14   |                      |       |       | CC1OF | <RES> |       |     |       |       |       |       | CC1IF | UIF
              *  15   |                      |       | CC2OF | CC1OF | <RES> | BIF   | TIF | COMIF |       |       | CC2IF | CC1IF | UIF
              */

             EGR,    // 0x14
             /*
              * Timer | 7   | 6   | 5   |  4    |  3    | 2     | 1     | 0
              * ------------------------------------------------------------------------
              *  1    | BG  | TG  | COM | CC4G  | CC3G  | CC2G  | CC1G  | UG
              *  2, 3 |     | TG  |     | CC4G  | CC3G  | CC2G  | CC1G  | UG
              *  14   |     |     |     |       |       |       | CC1G  | UG
              *  15   | BG  | TG  | COM |       |       | CC2G  | CC1G  | UG
              */

             CCMR1,  // 0x18 - unused in [6]
             /*
              * (____________________________ OUTPUT COMPARE MODE _______________________________________)
              * Timer    | 15    | 14 : 12 | 11    | 10    | 9 : 8 | 7     | 6 : 4 |  3    | 2     | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3 | OC2CE | OC2M    | OC2PE | OC2FE | CC2S  | OC1CE | OC1M  | OC1PE | OC1FE | CC1S
              *  14      |       |         |       |       |       |       | OC1M  | OC1PE | OC1FE | CC1S
              *  15      |       | OC2M    | OC2PE | OC2FE | CC2S  |       | OC1M  | OC1PE | OC1FE | CC1S
              *
              * (____________________________ INPUT CAPTURE MODE _______________________________________)
              * Timer        | 15 : 12 | 11 :10 |                9 : 8 | 7 : 4 |  3 : 2 | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3, 15 | IC2F    | IC2PSC |                 CC2S | IC1F  | IC1PSC | CC1S
              *  14          |         |        |                      | IC1F  | IC1PSC | CC1S
              */

             CCMR2,  // 0x1C - unused in [14, 15, 16, 17, 6]
             /*
              * (____________________________ OUTPUT COMPARE MODE _______________________________________)
              * Timer    | 15    | 14 : 12 | 11    | 10    | 9 : 8 | 7     | 6 : 4 |  3    | 2     | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3 | OC4CE | OC4M    | OC4PE | OC4FE | CC4S  | OC3CE | OC3M  | OC3PE | OC3FE | CC3S
              *
              * (____________________________ INPUT CAPTURE MODE _______________________________________)
              * Timer    | 15 : 12 | 11 :10 |                9 : 8 | 7 : 4 |  3 : 2 | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3 | IC4F    | IC4PSC |                 CC4S | IC3F  | IC3PSC | CC3S
              */

             CCER,   // 0x20 - unused in [6]
             /*
              * Timer | 15    | 14    | 13   | 12   | 11    | 10    | 9    | 8    | 7     | 6     | 5    | 4    | 3     | 2     | 1     | 0
              * ------------------------------------------------------------------------
              *  1    | <RES> | <RES> | CC4P | CC4E | CC3NP | CC3NE | CC3P | CC3E | CC2NP | CC2NE | CC2P | CC2E | CC1NP | CC1NE | CC1P | CC1E
              *  2, 3 | CC4NP | <RES> | CC4P | CC4E | CC3NP |       | CC3P | CC3E | CC2NP |       | CC2P | CC2E | CC1NP |       | CC1P | CC1E
              *  14   |       | <RES> |      |      |       |       |      |      |       |       |      |      | CC1NP |       | CC1P | CC1E
              *  15   |       | <RES> |      |      |       |       |      |      | CC2NP |       | CC2P | CC2E | CC1NP | CC1NE | CC1P | CC1E
              */

             CNT,    // 0x24 (16 bits), TIM2 uses (32 bits or top 16 bits?)
             PSC,    // 0x28 (16 bits)
             ARR,    // 0x2C (16 bits), TIM2 uses (32 bits or top 16 bits?)

             RCR,    // 0x30 - unused in [2, 3, 14, 6] (8 bits)

             CCR1,   // 0x34 - unused in [6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR2,   // 0x38 - unused in [14, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR3,   // 0x3C - unused in [14, 15, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR4,   // 0x40 - unused in [14, 15, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)

             BDTR,   // 0x44 - unused in [2, 3, 14, 6]
             /*
              * Timer  | 15  | 14  | 13  | 12  | 11   | 10   | 9 : 8 | 7 : 0
              * ------------------------------------------------------------
              *  1, 15 | MOE | AOE | BKP | BKE | OSSR | OSSI | LOCK  | DT
              */

             DCR,    // 0x48 - unused in [14, 6]
             /*
              * Timer        | 12 : 8 | 7 : 5 | 4 : 0
              * ------------------------------------------------------------------------
              *  1, 2, 3, 15 | DBL    | <RES> | DBA
              */

             DMAR,   // 0x4C - unused in [14, 6] (16 bits)
             OR;     // 0x50 - unused in [1, 2, 3, 15, 16, 17, 6]
             /*
              * Timer | 0
              * -----------
              *  14   | TI1_RMP
              */
} Timer;


#endif  // TIMER_H
