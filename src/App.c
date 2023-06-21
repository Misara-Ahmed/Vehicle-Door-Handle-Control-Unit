/******************************************************************************
 *
 * Author: Team 27
 *
 * Date: 18/6/2023
 *
 * Application: Vehicle Door Handle Control Unit
 * 
 * File Name: App.c (Source File)
 *
 *******************************************************************************/

#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Bit_Operations.h"
#include "TIM.h"


#define LOCKED 						0
#define UNLOCKED 					1

#define CLOSED 						0
#define OPENED						1

#define DOOR_UNLOCK_BTN				1
#define DOOR_HANDLE_BTN				0

#define VEHICLE_LED					0
#define HAZARD_LED					1
#define AMBIENT_LED					2

#define PRESSED 					0
#define RELEASED					1

#define ON							1
#define OFF							0

/*
 * Description: This function is used to perform an action on a specific LED
 * Inputs: The action to be performed (action)
 *         The address of the flag of the LED (flag)
 *         The LED to be controlled (led)
 * Return: void
*/
void ledAction(uint8 action, uint8 * flag, uint8 led);

/*
 * Description: This function is used to blink a specific LED
 * Inputs: The number of times the LED blinks (Num_Of_Times)
 *         The duration of each blink (On_duration)
 *         The LED to be controlled (led)
 *         The address of the flag of the LED (flag)
 *         The time elapsed by the timer (time)
 * Return: void
*/
void ledBlinking(uint8 Num_Of_Times, uint32 On_duration, uint8 led , uint8 * flag ,uint32 time);

void main(void)
{
	/* Enable clock */
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOB);
	Rcc_Enable(RCC_TIM4);
	GPT_Init();

	/* Buttons Configuration */
	Gpio_ConfigPin(GPIO_A,DOOR_UNLOCK_BTN,GPIO_INPUT,GPIO_PULL_UP);
	Gpio_ConfigPin(GPIO_A,DOOR_HANDLE_BTN,GPIO_INPUT,GPIO_PULL_UP);

	/* LEDs Configuration */
	Gpio_ConfigPin(GPIO_B,VEHICLE_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);
	Gpio_ConfigPin(GPIO_B,HAZARD_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);
	Gpio_ConfigPin(GPIO_B,AMBIENT_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);

	/* Initial state */
	uint8 Vehicle_Lock = LOCKED;
	uint8 Vehicle_Door = CLOSED;

	Gpio_WritePin(GPIO_B,VEHICLE_LED,LOW);
	Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
	Gpio_WritePin(GPIO_B,AMBIENT_LED,LOW);

	/* Flags */
	uint8 vehicle_led_flag = 0;
	uint8 hazard_led_flag = 0;
	uint8 ambient_led_flag = 0;
	uint8 closing = 0;
	uint8 no_btn_pressed = 0;
	uint8 handle_btn_flag = 0;
	uint8 lock_btn_flag = 0;

	while (1)
	{
		// Initial state where the vehicle is locked and the door is closed
		if (Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			// Check if the door handle button is pressed
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
			{
				// Wait for debouncing
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);

				if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
				{
					// Set the button flag to 1 to prevent multiple entering unless the button is released
					handle_btn_flag = 1;

					// Start the timer for 10 seconds
					GPT_StartTimer(10000);

					// Change the state of the vehicle to unlocked to enter the next state
					Vehicle_Lock = UNLOCKED;
				}
			}
			// Check if the button is released
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == RELEASED )
			{
				// Reset the button flag
				handle_btn_flag = 0;
			}
		}

		// Second state where the vehicle is unlocked and the door is closed
		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			// Check if no button is pressed
			if (no_btn_pressed == 0)
			{
				// Check if we elapsed 10 seconds without any button
				if (GPT_CheckTimeIsElapsed() == 1)
				{
					// Start timer for 2 seconds for the anti-theft system
					GPT_StartTimer(2000);

					// Set the flag to 1 to enter the anti-theft system
					no_btn_pressed = 1;
				}
			}

			// Check if the door lock button is pressed
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
			{
				// Wait for debouncing
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);

				if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
				{
					// Set the button flag to 1 to prevent multiple entering unless the button is released
					lock_btn_flag = 1;

					// Change the state of the vehicle door to opened to enter the next state					
					Vehicle_Door = OPENED;
				}
			}

			// Check if the button is released
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == RELEASED )
			{
				// Reset the button flag
				lock_btn_flag = 0;
			}

			// Check if the anti-theft system is activated
			if (no_btn_pressed == 1)
			{
				/*
				* Turn off the vehicle LED
				* Turn off the ambient LED
				* Blink the hazard LED for 2 seconds and then turn it off
				*/

				ledAction(OFF, &vehicle_led_flag, VEHICLE_LED);

				ledAction(OFF, &ambient_led_flag, AMBIENT_LED);

				ledBlinking( 2, 500, HAZARD_LED, &hazard_led_flag ,GPT_GetElapsedTime() );
				
				// Check if the 2 seconds elapsed
				if (GPT_CheckTimeIsElapsed() == 1)
				{
					ledAction(OFF, &hazard_led_flag, HAZARD_LED);

					// Reset the state of the vehicle to locked to renter the initial state
					Vehicle_Lock = LOCKED;

					// Reset the flag
					no_btn_pressed = 0;
				}
			}
			else
			{
				/*
				* Turn on the vehicle LED
				* Blink the hazard LED for 1 second
				* Turn on the ambient LED for 2 seconds and then turn it off
				*/

				ledAction(ON, &vehicle_led_flag, VEHICLE_LED);

				ledBlinking( 1, 500, HAZARD_LED, &hazard_led_flag ,GPT_GetElapsedTime() );

				if (GPT_GetElapsedTime() < 2000)
				{
					ledAction(ON, &ambient_led_flag, AMBIENT_LED);
				}
				if (GPT_GetElapsedTime() > 2000)
				{
					ledAction(OFF, &ambient_led_flag, AMBIENT_LED);
				}
			}
		}

		// Third state where the vehicle is unlocked and the door is opened
		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == OPENED)
		{
			/*
			* Turn on the ambient LED
			*/
			ledAction(ON, &ambient_led_flag, AMBIENT_LED);
			
			// Check if the door lock button is pressed
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
			{
				// Wait for debouncing
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);

				if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED &&  lock_btn_flag == 0)
				{
					// Set the button flag to 1 to prevent multiple entering unless the button is released
					lock_btn_flag = 1;
					
					// Start the timer for 1 second
					GPT_StartTimer(1000);

					// Change the state of the vehicle door to closed to enter the next state
					Vehicle_Door = CLOSED;

					// Set the closing flag to 1 where the door is closing
					closing = 1;
				}
			}

			// Check if the button is released
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == RELEASED )
			{
				// Reset the button flag
				lock_btn_flag = 0;
			}
		}

		// Fourth state where the vehicle is unlocked after closing the door
		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			/*
			* Turn off the vehicle LED
			* Turn off the hazard LED
			* Keep the ambient LED on for 1 second and then turn it off
			*/

			ledAction(OFF, &vehicle_led_flag, VEHICLE_LED);

			ledAction(OFF, &hazard_led_flag, HAZARD_LED);

			// Check if the 1 second elapsed
			if (GPT_CheckTimeIsElapsed() == 1)
			{
				ledAction(OFF, &ambient_led_flag, AMBIENT_LED);
			}
			
			// Check if the door handle button is pressed
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
			{
				// Wait for debouncing
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);

				if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
				{
					// Set the button flag to 1 to prevent multiple entering unless the button is released
					handle_btn_flag = 1;

					// Start the timer for 2 seconds
					GPT_StartTimer(2000);

					// Change the state of the vehicle lock to locked but with closing the door
					Vehicle_Lock = LOCKED;
					closing = 1;
				}
			}

			// Check if the button is released
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == RELEASED )
			{
				// Reset the button flag
				handle_btn_flag = 0;
			}
		}

		// Fifth state where the vehicle is locked after closing the door
		if (Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			/*
			* Turn off the ambient LED
			* Blink the hazard LED for 2 seconds
			* Keep the ambient LED on for 2 seconds and then turn it off
			*/
			ledAction(OFF, &ambient_led_flag, AMBIENT_LED);

			ledBlinking( 2, 500, HAZARD_LED, &hazard_led_flag ,GPT_GetElapsedTime() );

			// Check if the 2 seconds elapsed
			if (GPT_CheckTimeIsElapsed() == 1)
			{
				ledAction(OFF, &hazard_led_flag, HAZARD_LED);

				// Reset the closing flag
				closing = 0;
			}
		}
	}
}

/*
 * Description: This function is used to perform an action on a specific LED
 * Inputs: The action to be performed (action)
 *         The address of the flag of the LED (flag)
 *         The LED to be controlled (led)
 * Return: void
*/
void ledAction(uint8 action, uint8 * flag, uint8 led)
{
	if(action == ON)
	{
		if ( *flag == OFF )
		{
			Gpio_WritePin(GPIO_B,led,HIGH);
			*flag = ON;
		}
	}
	else
	{
		if ( *flag == ON )
		{
			Gpio_WritePin(GPIO_B,led,LOW);
			*flag = OFF;
		}
	}
}

/*
 * Description: This function is used to blink a specific LED
 * Inputs: The number of times the LED blinks (Num_Of_Times)
 *         The duration of each blink (On_duration)
 *         The LED to be controlled (led)
 *         The address of the flag of the LED (flag)
 *         The time elapsed by the timer (time)
 * Return: void
*/
void ledBlinking(uint8 Num_Of_Times, uint32 On_duration, uint8 led , uint8 * flag ,uint32 time)
{
	static uint8 itr = 1;
	static uint8 itr2 = 1;
	if(itr2 <= Num_Of_Times)
	{
		if ( time < On_duration*itr && time > On_duration*(itr-1) )
		{
			ledAction(ON, flag, led);
		}
		else
		{
			ledAction(OFF, flag, led);
			itr = itr+2;
			itr2++;
		}
	}
	else
	{
		itr = 1;
		itr2 = 1;
	}
}

