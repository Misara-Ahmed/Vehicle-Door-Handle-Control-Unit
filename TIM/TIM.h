/******************************************************************************
 *
 * Author: Misara Ahmed
 *
 * Date: 6/6/2023
 *
 * Driver: GENRAL PURPOSE TIMER
 * 
 * Description: This driver is used to configure the general purpose timer (GPT)
 * 
 * File Name: TIM.h (Header File)
 *
 *******************************************************************************/

#ifndef TIM_H_
#define TIM_H_

#include "Std_Types.h"

#define TIM4_BASE_ADDRESS 0x40000800
#define TIM4_CR1 *((uint16 *)(TIM4_BASE_ADDRESS + 0x00))
#define TIM4_CR2 *((uint16 *)(TIM4_BASE_ADDRESS + 0x04))
#define TIM4_SMCR *((uint16 *)(TIM4_BASE_ADDRESS + 0x08))
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
#define TIM4_PRESCALER                          4100

/* 
 * Description: This function is used to initialize and configure the general purpose timer (GPT)
 * Inputs: void
 * Return: void
 */
void GPT_Init(void);

/* 
 * Description: This function is used to start the general purpose timer (GPT)
 * Inputs: The number of ticks after which the timer overflows (OverFlowTicks)
 * Return: void
 */
void GPT_StartTimer(uint32 OverFlowTicks);

/* 
 * Description: This function is used to check if the general purpose timer (GPT) has finished counting
 * Inputs: void
 * Return: 1 if the timer has finished counting
 *         0 otherwise
 */
uint8 GPT_CheckTimeIsElapsed(void);

/* 
 * Description: This function is used to get the elapsed time (counts) since the general purpose timer (GPT) has started
 * Inputs: void
 * Return: The elapsed time in ticks.
 */
uint32 GPT_GetElapsedTime(void);

/* 
 * Description: This function is used to calculate the remaining time (counts) for the general purpose timer (GPT) to finish counting
 * Inputs: void
 * Return: The remaining time in ticks.
 */
uint32 GPT_GetRemainingTime(void);

#endif /* TIM_H_ */
