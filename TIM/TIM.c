/*
 * TIM.c
 *
 *  Created on: Jun 6, 2023
 *      Author: Misara
 */

#include "TIM.h"
#include "RCC.h"
#include "GPIO.h"

void GPT_Init(void)
{
    // Set the prescaler value (PSC)    
    TIM4_PSC = TIM4_PRESCALER - 1;

    // Clear the CMS bits and set the edge-aligned mode to be depending on the direction bit
    TIM4_CR1 &= ~(3 << 5);

    // Set the direction (DIR) bit to be up-counting
    TIM4_CR1 &= ~(1 << 4);

    // Disable the auto-reload preload (ARPE)
    //TIM4_CR1 &= ~(1 << 7);

    //TIM4_CR1 |= (1 << 7);

}

void GPT_StartTimer(uint32 OverFlowTicks)
{
    // Set the auto-reload value (ARR)
    TIM4_ARR = OverFlowTicks;

    // Enable the counter (CEN bit)
    TIM4_CR1 |= (1 << 0);

    // Make the counter register (CNT) start from 1 as the counter start after one clock cycle
    TIM4_CNT = 1;
}

uint8 GPT_CheckTimeIsElapsed(void)
{
    // Check if the counter (CNT) has reached the auto-reload value (ARR)
    if( TIM4_CNT == TIM4_ARR-1 )
    {
    	TIM4_CNT = 0;

    	TIM4_ARR = 1;

    	// Disable the counter (CEN bit)
    	TIM4_CR1 &= ~(1 << 0);

        return 1;
    }
    else
    {
        return 0;
    }
}

uint32 GPT_GetElapsedTime(void)
{
    // Return the current value of the counter (CNT) which represents the time in ticks (msec)
    return TIM4_CNT;
}

uint32 GPT_GetRemainingTime(void)
{
    // Return the remaining time to overflow (ARR - CNT) which represents the remaining time in ticks (msec)
    return (TIM4_ARR - TIM4_CNT);
}
