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
 * File Name: TIM.c (Source File)
 *
 *******************************************************************************/

#include "TIM.h"
#include "RCC.h"
#include "GPIO.h"

/* 
 * Description: This function is used to initialize and configure the general purpose timer (GPT)
 * Inputs: void
 * Return: void
 */
void GPT_Init(void)
{
    // Set the prescaler value (PSC)    
    TIM4_PSC = TIM4_PRESCALER - 1;

    // Clear the CMS bits and set the edge-aligned mode to be depending on the direction bit
    TIM4_CR1 &= ~(3 << 5);

    // Clear the direction (DIR) bit to be up-counting
    TIM4_CR1 &= ~(1 << 4);

}

/* 
 * Description: This function is used to start the general purpose timer (GPT)
 * Inputs: The number of ticks after which the timer overflows (OverFlowTicks)
 * Return: void
 */
void GPT_StartTimer(uint32 OverFlowTicks)
{
    // Set the auto-reload value (ARR)
    TIM4_ARR = OverFlowTicks;

    // Enable the counter (CEN bit)
    TIM4_CR1 |= (1 << 0);

    // Make the counter register (CNT) start from 1 as the counter start after one clock cycle
    TIM4_CNT = 1;
}

/* 
 * Description: This function is used to check if the general purpose timer (GPT) has finished counting
 * Inputs: void
 * Return: 1 if the timer has finished counting
 *         0 otherwise
 */
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

/* 
 * Description: This function is used to get the elapsed time (counts) since the general purpose timer (GPT) has started
 * Inputs: void
 * Return: The elapsed time in ticks.
 */
uint32 GPT_GetElapsedTime(void)
{
    // Return the current value of the counter (CNT) which represents the time in ticks (msec)
    return TIM4_CNT;
}

/* 
 * Description: This function is used to calculate the remaining time (counts) for the general purpose timer (GPT) to finish counting
 * Inputs: void
 * Return: The remaining time in ticks.
 */
uint32 GPT_GetRemainingTime(void)
{
    // Return the remaining time to overflow (ARR - CNT) which represents the remaining time in ticks (msec)
    return (TIM4_ARR - TIM4_CNT);
}
