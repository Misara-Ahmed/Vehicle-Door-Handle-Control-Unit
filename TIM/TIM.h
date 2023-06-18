/*
 * TIM.h
 *
 *  Created on: Jun 6, 2023
 *      Author: Misara
 */

#ifndef TIM_H_
#define TIM_H_

#include "Std_Types.h"

#define TIM4_BASE_ADDRESS 0x40000800
#define TIM4_CR1 *((uint16 *)(TIM4_BASE_ADDRESS + 0x00))
//#define TIM4_CR2 *((uint16 *)(TIM4_BASE_ADDRESS + 0x04))
//#define TIM4_SMCR *((uint16 *)(TIM4_BASE_ADDRESS + 0x08))
#define TIM4_DIER *((uint16 *)(TIM4_BASE_ADDRESS + 0x0C))
#define TIM4_SR *((uint16 *)(TIM4_BASE_ADDRESS + 0x10))
#define TIM4_EGR *((uint16 *)(TIM4_BASE_ADDRESS + 0x14))
#define TIM4_CNT *((uint16 *)(TIM4_BASE_ADDRESS + 0x24))
#define TIM4_PSC *((uint16 *)(TIM4_BASE_ADDRESS + 0x28))
#define TIM4_ARR *((uint16 *)(TIM4_BASE_ADDRESS + 0x2C))
#define TIM4_CCR1 *((uint16 *)(TIM4_BASE_ADDRESS + 0x34))
#define TIM4_CCR2 *((uint16 *)(TIM4_BASE_ADDRESS + 0x38))
#define TIM4_CCR3 *((uint16 *)(TIM4_BASE_ADDRESS + 0x3C))
#define TIM4_CCR4 *((uint16 *)(TIM4_BASE_ADDRESS + 0x40))

#define MCU_CLOCK                               16000000
#define TIM4_PRESCALER                          16000

//#define TIM4_TICK_TIME                          1
//#define TIM4_TICKS_PER_MS                       (TIM4_PRESCALER / TIM4_TICK_TIME)

//#define TIM4 ((TIMRegs *)0x40000800)

// typedef struct 
// {
// 	uint16 CR1;
// 	uint16 CR2;
// 	uint16 SMCR;
// 	uint16 DIER;
// 	uint16 SR;
// 	uint16 EGR;
//     uint16 CCMR1;
//     uint16 CCMR2;
//     uint16 CCER;
//     uint32 CNT;
//     uint16 PSC;
//     uint32 ARR;
//     uint32 CCR1;
//     uint32 CCR2;
//     uint32 CCR3;
//     uint32 CCR4;
//     uint16 DCR;
//     uint16 DMAR;

// } TIMRegs;

void GPT_Init(void);

void GPT_StartTimer(uint32 OverFlowTicks);

uint8 GPT_CheckTimeIsElapsed(void);

uint32 GPT_GetElapsedTime(void);

uint32 GPT_GetRemainingTime(void);

#endif /* TIM_H_ */
