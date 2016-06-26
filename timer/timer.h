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
              * Timer      | 9 : 8 | 7    | 6 : 5 | 4   | 3   | 2   | 1    | 0
              * -----------------------------------------------------------------
              * 1, 2, 3    | CKD   | ARPE | CMS   | DIR | OPM | URS | UDIS | CEN
              * 14         | CKD   | ARPE |       |     |     | URS | UDIS | CEN
              * 15, 16, 17 | CKD   | ARPE |       |     | OPM | URS | UDIS | CEN
              * 6          |       | ARPE |       |     | OPM | URS | UDIS | CEN
              *
              * CKD (Clock Division)
              * - 00: t_DTS = t_CK_INT
              * - 01: t_DTS = 2 * t_CK_INT
              * - 10: t_DTS = 4 * t_CK_INT
              * - 11: INVALID
              * - t_DTS = deadtime and sampling clock, t_CK_INT = timer clock frequency
              *
              * ARPE (Auto-reload preload enable)
              * - 0: ARR is not buffered
              * - 1: ARR is buffered
              *
              * CMS (Center-aligned mode selection)
              * - 00: Edge-aligned (up or down depending on DIR)
              * - 01: Center-aligned mode 1
              * - 10: Center-aligned mode 2
              * - 11: Center-aligned mode 3
              *   - Center-aligned counts up / down alternatively, output
              *   compare interrupt flags of channels configured in output (CCxS == 00 in CCMRx) are set when
              *   - 1: when counting down
              *   - 2: when counting up
              *   - 3: when counting up or down
              * - __NOTE__: can't switch from edge-aligned to center-aligned while CEN is enabled
              *
              * DIR (Direction)
              * - 0: up
              * - 1: down
              * - __NOTE__: Read only when CMS > 0 or in encoder mode
              *
              * OPM (One pulse mode)
              * - 0: Counter not stopped at update event
              * - 1: Counter stopped at event, clearing CEN
              *
              * URS (Update request source): Generate update interrupt or DMA request
              * - 0: [
              *        counter overflow/underflow,
              *        setting the UG bit,
              *        update gen through slave mode controller
              *      ]
              * - 1: [ counter overflow/underflow ]
              *
              * UDIS (Update Disable)
              * - 0: Update events are generated by any source specified in URS
              *      then buffered registers are then loaded with their preload
              *      values
              * - 1: Update disabled, (ARR, PSC, CCRx) keep value, counter and
              *      prescalar get reset if UG is set or a hardware reset is
              *      received
              *
              * CEN (Counter Enable)
              * - 0: Disabled
              * - 1: Enabled
              * - __NOTE__: Trigger mode can set CEN,
              *   while (external clk, gated, and encoder) need CEN to be set by software
              */

             CR2,    // 0x04 - unused in [14]
             /*
              * Timer   | 14   | 13    | 12   | 11    | 10   | 9     | 8    | 7    | 6 : 4 |  3   | 2    | 1     | 0
              * -----------------------------------------------------------------------------------------------------
              *  1      | OIS4 | OIS3N | OIS3 | OIS2N | OIS2 | OIS1N | OIS1 | TI1S | MMS   | CCDS | CCUS | <RES> | CCPC
              *  2, 3   |                                                   | TI1S | MMS   | CCDS |
              *  15     |      |       |      |       | OIS2 | OIS1N | OIS1 |      | MMS   | CCDS | CCUS |       | CCPC
              *  16, 17 |      |       |      |       |      | OIS1N | OIS1 |      |       | CCDS | CCUS |       | CCPC
              *  6      |      |       |      |       |      |       |      |      | MMS   |      |      |       |
              *
              *  OISx[N] (Output Idle State)
              *  - 0: OC1[N]=0 afer a dead-time when MOE = 0
              *  - 1: OC1[N]=1 afer a dead-time when MOE = 0
              *  - __NOTE__: Not writeable when LOCK lvl > 0 in BDTR
              *
              *  TI1S (TI1 Selection)
              *  - 0: CH1 pin is connected to TI1 Input
              *  - 1: CH[1, 2, 3] XOR'd to TI1 Input
              *
              *  MMS (Master Mode Selection)
              *  - Select info to send to slaves for synchronization (TRGO)
              *  - 000: RESET     - UG bit from EGR, if generated from trigger
              *                     input, signal on TRGO is delayed compared
              *                     to reset
              *  - 001: ENABLE    - CEN, slight delay on trigger
              *  - 010: UPDATE    - UEV (Update event, useful for prescalar)
              *  - 011: CMP Pulse - CC1IF set __even when already high__
              *  - 100: CMP       - OC1REF
              *  - 101: CMP       - OC2REF
              *  - 110: CMP       - OC3REF
              *  - 111: CMP       - OC4REF
              *
              *  CCDS (Capture/Compare DMA Selection)
              *  - 0: CCx DMA Request sent when CCx event occurs
              *  - 1: CCx DMA Request sent when UEV occurs
              *
              *  CCUS (Capture/Compare control update selection)
              *  - When capture/compare bits are preloaded (CCPC=1) they are
              *  updated by:
              *  - 0: setting the COMG bit
              *  - 1: setting the COMG bit or rising edge on TRGI
              *  - __NOTE__: Only works on channels w/ complementary output
              *
              *  CCPC (Capture/Compare Preloaded Control)
              *  - 0: CCxE, CCxNE, OCxM not preloaded
              *  - 1: CCxE, CCxNE, OCxM are preloaded, after being written only
              *       updated when a COM event occurs (defined by CCUS)
              *  - __NOTE__: Only works on channels w/ complementary output
              */

             SMCR,   // 0x08 - unused in [14, 16, 17, 6] (Slave Mode Control Register)
             /*
              * Timer    | 15  | 14   | 13 : 12 | 11 : 8 | 7    | 6 : 4 |  3    | 2 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3 | ETP | ECE  | ETPS    | ETF    | MSM  | TS    | <RES> | SMS
              *  15      |     |      |         |        | MSM  | TS    | <RES> | SMS
              *
              *  ETP (External Trigger Polarity)
              *  - 0: ETR used at high-level or rising edge for trigger ops
              *  - 1: ETR used at low-level or falling edge for trigger ops
              *
              *  ECE (External Clock Enable)
              *  - 0: External Clock Mode 2 Disabled
              *  - 1: External Clock Mode 2 Enabled (Counter clocked by active
              *       edge on ETRF Signal)
              *  - __NOTES__:
              *    - ECE=1 ~ External clk md 1 with TRGI connected to ETRF (SMS=111, TS=11)
              *    - ECE can = 1 in slave modes [reset, gated, trigger], but
              *      TRGI can't connect to ETRF (TS != 111)
              *    - If ext_clk_md_1 and ext_clk_md_2 are enabled at the same
              *      time, ETRF is input
              *
              *  ETPS (External Trigger Prescaler)
              *  - ETRP freq. divided by 2^(ETPS)
              *  - __NOTE__: ETRP freq. must be <= CLK/4
              *
              *  ETF (External Trigger Filter)
              *  - define f_sampling of ETRP and length of filter (N) events
              *    needed to validate
              *       | Frequency  | N
              *  ---- | ---------- | ---
              *  0000 | f_dts      | 1
              *  0001 | f_ck_int   | 2
              *  0010 | f_ck_int   | 4
              *  0011 | f_ck_int   | 8
              *  0100 | f_dts / 2  | 6
              *  0101 | f_dts / 2  | 8
              *  0110 | f_dts / 4  | 6
              *  0111 | f_dts / 4  | 8
              *  1000 | f_dts / 8  | 6
              *  1001 | f_dts / 8  | 8
              *  1010 | f_dts / 16 | 5
              *  1011 | f_dts / 16 | 6
              *  1100 | f_dts / 16 | 8
              *  1101 | f_dts / 32 | 5
              *  1110 | f_dts / 32 | 6
              *  1111 | f_dts / 32 | 8
              *
              *  MSM (Master / Slave Mode)
              *  - 0: Nothing
              *  - 1: Effect of event on TRGI is delayed to allow
              *       synchronization b/t current timer and slaves w/ (TRGO)
              *
              *  TS (Trigger Selection)
              *  - Trigger input to synchronize the timer
              *  - 000: ITR0 (Internal Trigger) (TIM15)
              *  - 001: ITR1 (TIM2)
              *  - 010: ITR2 (TIM3)
              *  - 011: ITR3 (TIM17)
              *  - 100: TI1F_ED (TI1 Edge Detector)
              *  - 101: TI1FP1 (Filtered Time Input)
              *  - 110: TI2FP2
              *  - 111: ETRF (External Trigger Input)
              *
              *  SMS (Slave Mode Selection)
              *  - When ext. signals selected, the active edge of TRGI is linked
              *    to polarity selected on external input
              *  - 000: Slave mode disabled, if CEN=1 prescalar clocked by
              *         internal clock
              *  - 001: Encoder mode 1, CTR goes up/down on TI2FP2 edge dep. on TIF1P1 level
              *  - 010: Encoder mode 2, CTR goes up/down on TI1FP1 edge dep. on TIF2P2 level
              *  - 011: Encoder mode 3, CTR goes up/down on both edges dep. on other input
              *  - 100: Reset mode, Rising edge of TRGI reinitializes the counter and update of the registers
              *  - 101: Gated mode, Counter Clock Enabled when TRGI is high, stopped when low
              *  - 110: Trigger Mode, Only started at rising edge of TRGI
              *  - 111: External Clock Mode 1, Rising edge of TRGI clock the counter
              *  - __NOTE__: Can't use gated mode if TI1F_ED is TRGI (TS=100)
              *              because only pulse generated
              */

             DIER,   // 0x0C (DMA Interrupt Enable Register)
             /*
              * Timer   | 14   | 13    | 12    | 11    | 10    | 9     | 8   | 7       | 6   | 5     |  4    |  3    | 2     | 1     | 0
              * -------------------------------------------------------------------------------------------------------------------------
              *  1      | TDE  | COMDE | CC4DE | CC3DE | CC2DE | CC1DE | UDE | BIE     | TIE | COMIE | CC4IE | CC3IE | CC2IE | CC1IE | UIE
              *  2, 3   | TDE  |       | CC4DE | CC3DE | CC2DE | CC1DE | UDE |         | TIE |       | CC4IE | CC3IE | CC2IE | CC1IE | UIE
              *  14     |      |       |       |       |       |       |     |         |     |       |       |       |       | CC1IE | UIE
              *  15     | TDE  |       |       |       | CC2DE | CC1DE | UDE | BIE     | TIE | COMIE |       |       | CC2IE | CC1IE | UIE
              *  16, 17 | TDE  |       |       |       |       | CC1DE | UDE | BIE     | TIE | COMIE |       |       |       | CC1IE | UIE
              *  6      |      |       |       |       |       |       | UDE |         |     |       |       |       |       |       | UIE
              *
              *  TDE (Trigger DMA Request Enable)
              *  COMDE (COM DMA Request Enable)
              *  CCxDE (Capture/Compare x DMA Request Enable)
              *  UDE (Update DMA Request Enable)
              *
              *  BIE (Break Interrupt Enable)
              *  TIE (Trigger Interrupt Enable)
              *  COMIE (COM Interrupt Enable)
              *  CCxIE (Capture/Compare Interrupt Enable)
              *  UIE (Update Interrupt Enable)
              */

             SR,     // 0x10 (Status Register)
             /*
              * Timer   |                12    | 11    | 10    | 9     | 8     | 7     | 6   | 5     |  4    |  3    | 2     | 1     | 0
              * -------------------------------------------------------------------------------------------------------------------------
              *  1      |                CC4OF | CC3OF | CC2OF | CC1OF | <RES> | BIF   | TIF | COMIF | CC4IF | CC3IF | CC2IF | CC1IF | UIF
              *  2, 3   |                CC4OF | CC3OF | CC2OF | CC1OF | <RES> |       | TIF | COMIF | CC4IF | CC3IF | CC2IF | CC1IF | UIF
              *  14     |                      |       |       | CC1OF | <RES> |       |     |       |       |       |       | CC1IF | UIF
              *  15     |                      |       | CC2OF | CC1OF | <RES> | BIF   | TIF | COMIF |       |       | CC2IF | CC1IF | UIF
              *  16, 17 |                      |       |       | CC1OF | <RES> | BIF   | TIF | COMIF |       |       |       | CC1IF | UIF
              *  6      |                      |       |       |       |       |       |     |       |       |       |       |       | UIF
              *
              *  CCxOF (Capture/Compare x Overcapture Flag)
              *  - Only when corresponding channel in input capture mode,
              *    cleared by SW writing a 0
              *  - 0: No overcapture detected
              *  - 1: Counter value has been captured in CCRx while CCxIF flag was set
              *
              *  BIF (Break Interrupt Flag)
              *  - set by HW when break input is active, only clearable by SW when no longer active
              *
              *  TIF (Trigger Interrupt Flag)
              *  - HW set on active edge of TRGI when slave mode not gated
              *
              *  COMIF (COM Interrupt Flag)
              *  - Set on COM event (Capture/Compare ctrl bits - CCxE, CCxNE, OCxM have been updated)
              *
              *  CCxIF (Capture/Compare x Interrupt Flag)
              *  - When CCx is OUTPUT:
              *    - Set when counter matches compare val (w/ some exception in center-aligned mode)
              *    - 1: CNT = CCRx, when CCRx > ARR CCxIF goes high on overflow
              *         (in upcounting and up/down-counting mode) or underflow (in downcounting mode)
              *  - When CCx is INPUT:
              *    - Set on capture (edge on ICx with right polarity), cleared by SW, or reading CCRx
              *
              *  UIF (Update Interrupt Flag)
              *  - 1: Update interrupt pending, set by HW when registers are
              *  updated.
              *  - Conditions for UEV:
              *    - Overflow or underflow if rep. counter = 0 & UDIS=0 in CR1
              *    - CNT reinitialized by SW using UG bit in EGR, if URS = 0 & UDIS = 0 in CR1
              *    - CNT reinitialized by a trigger event if URS = 0 & UDIS = 0
              */

             EGR,    // 0x14 (Event Generation Register, generates interrupt / DMA and sets bits)
             /*
              * Timer   | 7   | 6   | 5    |  4    |  3    | 2    | 1     | 0
              * --------------------------------------------------------------------------
              *  1      | BG  | TG  | COMG | CC4G  | CC3G  | CC2G | CC1G  | UG
              *  2, 3   |     | TG  |      | CC4G  | CC3G  | CC2G | CC1G  | UG
              *  14     |     |     |      |       |       |      | CC1G  | UG
              *  15     | BG  | TG  | COMG |       |       | CC2G | CC1G  | UG
              *  16, 17 | BG  | TG  | COMG |       |       |      | CC1G  | UG
              *  6      |     |     |      |       |       |      |       | UG
              *
              *  BG (Break Generation)
              *  - MOE bit cleared, BIF set
              *
              *  TG (Trigger Generation)
              *  - TIF set
              *
              *  COMG (COM Generation)
              *  - if CCPC is set, allows to update CCxE, CCxNE, OCxM
              *  - __NOTE__: Only on channels w/ complementary output
              *
              *  CCxG (Capture/Compare x Generation)
              *  - When CCx is OUTPUT: CCxIF is set
              *  - When CCx is INPUT: Counter captured in CCRx, CCxIF is set and
              *                       if CCxIF was already set, CCxOF is set
              *
              *  UG (Update Generation)
              *  - Update the registers
              *  - Counter is reinitialized
              *    - cleared if center-aligned mode or if DIR=0
              *    - takes ARR if DIR=1 (downcounting)
              *  - Prescaler counter is cleared too
              */

             CCMR1,  // 0x18 - unused in [6] (Capture/Compare Mode Register)
             /*
              * (____________________________ OUTPUT COMPARE MODE _______________________________________)
              * Timer       | 15    | 14 : 12 | 11    | 10    | 9 : 8 | 7     | 6 : 4 |  3    | 2     | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3    | OC2CE | OC2M    | OC2PE | OC2FE | CC2S  | OC1CE | OC1M  | OC1PE | OC1FE | CC1S
              *  14, 16, 17 |       |         |       |       |       |       | OC1M  | OC1PE | OC1FE | CC1S
              *  15         |       | OC2M    | OC2PE | OC2FE | CC2S  |       | OC1M  | OC1PE | OC1FE | CC1S
              *
              * (____________________________ INPUT CAPTURE MODE _______________________________________)
              * Timer        | 15 : 12 | 11 :10 |                9 : 8 | 7 : 4 |  3 : 2 | 1 : 0
              * ---------------------------------------------------------------------------------------------------
              *  1, 2, 3, 15 | IC2F    | IC2PSC |                 CC2S | IC1F  | IC1PSC | CC1S
              *  14, 16, 17  |         |        |                      | IC1F  | IC1PSC | CC1S
              *
              *  __OUTPUT__
              *
              *  OCxCE (Output Compare x Clear Enable)
              *  - 0: OCxREF is not affected by ETRF
              *  - 1: OCxREF is cleared w/ HIGH ETRF
              *
              *  OCxM (Output Compare x Mode)
              *  - defines output of OCxREF (OCx and OCxN are derived from),
              *    OCxREF is active high. Active level of OCx and OCxN depend on
              *    CCxP and CCxNP
              *  - 000: Frozen - Comparison between CCRx and CNT doesn't change
              *                  output (used for generating timing base)
              *  - 001: Set chx HIGH when CCRx == CNT
              *  - 010: Set chx LOW when CCRx == CNT
              *  - 011: Toggle chx when CCRx == CNT
              *  - 100: Force LOW
              *  - 101: Force HIGH
              *  - 110: PWM Mode 1
              *    - upcounting:  chx HIGH while CNT < CCRx, else LOW
              *    - downcounting: chx LOW while CNT > CCRx, else HIGH
              *  - 111: PWM Mode 2
              *    - upcounting:    chx LOW while CNT < CCRx, else HIGH
              *    - downcounting: chx HIGH while CNT > CCRx, else LOW
              *  - __NOTES__:
              *    - Can't write while LOCK_lvl == 3 in BDTR & output configured (CCxS = 00)
              *    - In PWM modes, OCxREF only changes when comparison result
              *      changes or output compare mode switch from Frozen to PWM
              *    - Channels w/ compl. output this bitfield is preloaded. If
              *      CCPC is set in CR2, then OCxM active bits take new value
              *      fro mthe preloaded bits only when a COM event is generated
              *
              *  OCxPE (Output Compare x Preload Enable)
              *  - 0: Preload register on CCRx disabled. New writes to CCRx are
              *       used immediately
              *  - 1: R/W access the preloaded register, set to active at each
              *       update event
              *  - __NOTES__:
              *    - Can't be modified when LOCK_lvl = 3 and CCxS = 00
              *    - PWM Mode can be used w/o validating preload register only
              *      in one pulse mode (OPM set in CR1), otherwise undefined
              *
              *  OCxFE (Output Compare x Fast Enable)
              *  - 0: Minimum delay to activate CCx out is 5 clock cycles
              *  - 1: Reduced to 3 clock cycles, only works in PWM Mode
              *
              *  __INPUT__
              *
              *  ICxF (Input Capture x Filter)
              *  - Same filter as ETF
              *
              *  ICxPSC (Input Capture x Prescaler)
              *  - Capture done every (2 ^ ICxPSC) events
              *
              *  __COMMON__
              *
              *  CCxS(Capture/Compare x Selection)
              *  - 00: OUTPUT
              *  - 01: INPUT (ICx mapped to TIx)
              *  - 10: INPUT (ICx mapped to TI<x_other>), e.g.) x = 1, x_other =  2
              *  - 11: INPUT (ICx mapped to TRC, only works if internal trigger selected through TS bit on SMCR)
              *  - __NOTE__: only writeable when channel is off (CCxE = 0 in CCER)
              *
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
              *
              *  SEE CCMR1
              */

             CCER,   // 0x20 - unused in [6] (Capture/Compare Enable Register)
             /*
              * Timer   | 15    | 14    | 13   | 12   | 11    | 10    | 9    | 8    | 7     | 6     | 5    | 4    | 3     | 2     | 1     | 0
              * --------------------------------------------------------------------------
              *  1      | <RES> | <RES> | CC4P | CC4E | CC3NP | CC3NE | CC3P | CC3E | CC2NP | CC2NE | CC2P | CC2E | CC1NP | CC1NE | CC1P | CC1E
              *  2, 3   | CC4NP | <RES> | CC4P | CC4E | CC3NP |       | CC3P | CC3E | CC2NP |       | CC2P | CC2E | CC1NP |       | CC1P | CC1E
              *  14     |       | <RES> |      |      |       |       |      |      |       |       |      |      | CC1NP |       | CC1P | CC1E
              *  15     |       | <RES> |      |      |       |       |      |      | CC2NP |       | CC2P | CC2E | CC1NP | CC1NE | CC1P | CC1E
              *  16, 17 |       | <RES> |      |      |       |       |      |      |       |       |      |      | CC1NP | CC1NE | CC1P | CC1E
              *
              * __NOTE__:
              *      - Channels w/ complementary output, these bits are
              *        preloaded. If CCPC bit is set in CR2, then these get value
              *        only when Commutation event is generated
              *
              *  CCxNP (Capture/Compare x Complementary Output Polarity)
              *  - __OUTPUT__
              *    - 0: OCxN active high
              *    - 1: OCxN active low
              *
              *  - __INPUT__
              *    - Used w/ CCxP to define polarity of TI1FPx and TI2FPx, refer
              *      to CCxP
              *    - __NOTE__: Not writable when LOCK >= 2 and CCxS = 00
              *
              *  CCxNE (Capture/Compare x Complementary Output Enable)
              *  - 0: OCxN is not active. OCxN level is fxn of MOE, OSSI, OSSR, OISx, OISxN, & CCxE
              *  - 1: OCxN output to pin depending on bits shown above
              *
              *  CCxP (Capture/Compare x Output Polarity)
              *  - __OUTPUT__
              *    - 0: OCx active high
              *    - 1: OCx active low
              *  - __INPUT__ (Combine with CCxNP to define polarity of TI1FPx and TI2FPx)
              *    - (CCxNP,CCxP)
              *    - 00: NON-INVERTED (gated or encoder) RISING-EDGE (reset, external clk, trigger)
              *    - 01: INVERTED     (                ) FALLING-EDGE (                           )
              *    - 10: RESERVED __DO NOT USE__
              *    - 11: NON-INVERTED, both edges. MUST NOT USE in encoder mode
              *    - __NOTE__: Not Writable when LOCK >= 2
              *
              *  CCxE (Capture/Compare x Output Enable)
              *  - __OUTPUT__:
              *    - See CCxNE, substitute OCx for OCxN
              *  - __INPUT__:
              *    - 0: Capture of CNT into CCRx disabled
              *    - 1: Capture of CNT into CCRx enabled
              *
              *  See Table 45 (P 282) of reference manual
              */

             CNT,    // 0x24 (16 bits), TIM2 uses (32 bits or top 16 bits?)
             PSC,    // 0x28 (16 bits, Prescaler, ck_cnt = f_ck_psc / (PSC[15:0] + 1), reloaded on UEV)
             ARR,    // 0x2C (16 bits), TIM2 uses (32 bits or top 16 bits?, auto-reload register, counter blocked when null)

             RCR,    // 0x30 - unused in [2, 3, 14, 6] (8 bits)
             /*
              * Repetition Counter Register
              * Setup the update rate of the compare registers (transfer from
              * preload to active regs), when REP_CNT = 0, UEV is generated
              */

             CCR1,   // 0x34 - unused in [6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR2,   // 0x38 - unused in [14, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR3,   // 0x3C - unused in [14, 15, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             CCR4,   // 0x40 - unused in [14, 15, 16, 17, 6] (16 bits), TIM2 uses (32 bits or top 16 bits?)
             /*
              */

             BDTR,   // 0x44 - unused in [2, 3, 14, 6] (Break and Dead-Time Register)
             /*
              * Timer          | 15  | 14  | 13  | 12  | 11   | 10   | 9 : 8 | 7 : 0
              * ---------------------------------------------------------------------
              *  1, 15, 16, 17 | MOE | AOE | BKP | BKE | OSSR | OSSI | LOCK  | DTG
              *
              *  __NOTE__: These can get write-locked, so set all bits on first
              *            write to be careful
              *
              * MOE (Main Output Enable)
              * - 0: OC and OCN outputs are disabled or forced to idle state
              * - 1: OC and OCN outputs are enabled if the respective enable
              *      bits are set (CCxE and CCxNE in CCER)
              *
              * AOE (Automatic Output Enable)
              * - 0: MOE can only be set by SW
              * - 1: MOE can be set by SW or automatically at next UEV (if break input is not active)
              * - __NOTE__: Can't set when LOCK_lvl = 1
              *
              * BKP (Break Polarity)
              * - 0: Break input BRK is active LOW
              * - 1: Break input BRK is active HIGH
              * - __NOTE__: Can't set when LOCK_lvl = 1, delay of 1 APB clock cycle when writing
              *
              * BKE (Break Enable)
              * - 0: Break inputs (BRK and CCS clock failure event) disabled
              * - 1: Break inputs (BRK and CCS clock failure event) enabled
              * - __NOTE__: Can't set when LOCK_lvl = 1, delay of 1 APB clock cycle when writing
              *
              * OSSR (Off-state Selection for Run Mode)
              * - Used when MOE = 1 & channel has complementary output
              * - 0: When inactive, OC/OCN outputs are disabled
              * - 1: When inactive, OC/OCN outputs are enabled with inactive lvl as soon as CCxE=1 or CCxNE=1,
              *      then OC/OCN enable output signal=1
              * - __NOTE__: Can't set when LOCK_lvl = 2
              *
              * OSSI (Off-state Selection for Idle Mode)
              * - Used when MOE = 0 & channel has complementary output
              * - 0: When inactive, OC/OCN outputs are disabled
              * - 1: When inactive, OC/OCN outputs are forced first with idle lvl as soon as CCxE=1 or CCxNE=1,
              *      then OC/OCN enable output signal=1
              *
              * LOCK (Lock Configuration)
              * - 00: OFF
              * - 01: Level 1 - Can't write [BKP, BKE, AOE, DTG in BDTR, OISx and OISxN in CR2]
              * - 10: Level 2 - Level 1 + [OSSR, OSSI in BDTR and (CC Polarity bits) CCxP, CCxNP in CERR as long as channel configured in output through CCxS)
              * - 11: Level 3 - Level 2 + CC Control bits (OCxM and OCxPE in CCMRx as long as channel configured in output through CCxS)
              * - __NOTE__: Can only be written once after reset
              *
              * DTG (Dead-time Generator Setup)
              * - Defines duration of dead-time between complementary outputs,
              *   aka DT
              * - For DTG[7:5]
              *   - 0xx: DT = DTG[7:0] * t_DTS
              *   - 10x: DT = (64 + DTG[5:0]) * 2 * t_DTS
              *   - 110: DT = (32 + DTG[4:0]) * 8 * t_DTS
              *   - 111: DT = (32 + DTG[4:0]) * 16 * t_DTS
              * - __NOTE__: Can't be modified if LOCK > 0
              *
              */

             DCR,    // 0x48 - unused in [14, 6]
             /*
              * Timer                | 12 : 8 | 7 : 5 | 4 : 0
              * ------------------------------------------------------------------------
              *  1, 2, 3, 15, 16, 17 | DBL    | <RES> | DBA
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
