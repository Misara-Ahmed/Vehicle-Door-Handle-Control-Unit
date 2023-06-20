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

void ledAction(uint8 action, uint8 * flag, uint8 led);

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

	uint8 vehicle_led_flag = 0;
	uint8 hazard_led_flag = 0;
	uint8 ambient_led_flag = 0;
	uint8 closing = 0;
	//uint8 locking = 0;
	uint8 no_btn_pressed = 0;
	uint8 handle_btn_flag = 0;
	uint8 lock_btn_flag = 0;



	while (1)
	{
		if (Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
			{
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);
				if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
				{
					handle_btn_flag = 1;
					GPT_StartTimer(10000);
					Vehicle_Lock = UNLOCKED;
				}
			}
			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == RELEASED )
			{
				handle_btn_flag = 0;
			}
		}
		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			if (no_btn_pressed == 0)
			{
				if (GPT_CheckTimeIsElapsed() == 1)
				{
					GPT_StartTimer(2000);
					no_btn_pressed = 1;
				}
			}
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
			{
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);
				if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
				{
					lock_btn_flag = 1;
					Vehicle_Door = OPENED;
				}
			}
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == RELEASED )
			{
				lock_btn_flag = 0;
			}

			if (no_btn_pressed == 1)
			{
				ledAction(OFF, &vehicle_led_flag, VEHICLE_LED);

				ledAction(OFF, &ambient_led_flag, AMBIENT_LED);

				ledBlinking( 2, 500, HAZARD_LED, &hazard_led_flag ,GPT_GetElapsedTime() );

				if (GPT_CheckTimeIsElapsed() == 1)
				{
					ledAction(OFF, &hazard_led_flag, HAZARD_LED);
					Vehicle_Lock = LOCKED;
					no_btn_pressed = 0;
				}
			}
			else
			{
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

		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == OPENED)
		{
			ledAction(ON, &ambient_led_flag, AMBIENT_LED);

			//GPT_StartTimer(100);
			while (GPT_CheckTimeIsElapsed() != 1);
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED && lock_btn_flag == 0)
			{
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);
				if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED &&  lock_btn_flag == 0)
				{
					lock_btn_flag = 1;
					GPT_StartTimer(1000);
					Vehicle_Door = CLOSED;
					closing = 1;
				}
			}
			if ( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == RELEASED )
			{
				lock_btn_flag = 0;
			}
		}
		if (Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			ledAction(OFF, &vehicle_led_flag, VEHICLE_LED);

			ledAction(OFF, &hazard_led_flag, HAZARD_LED);

			if (GPT_CheckTimeIsElapsed() == 1)
			{
				ledAction(OFF, &ambient_led_flag, AMBIENT_LED);
			}

			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
			{
				GPT_StartTimer(30);
				while (GPT_CheckTimeIsElapsed() != 1);
				if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_btn_flag == 0)
				{
					handle_btn_flag = 1;
					GPT_StartTimer(2000);
					Vehicle_Lock = LOCKED;
					closing = 1;
				}
			}

			if ( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == RELEASED )
			{
				handle_btn_flag = 0;
			}
		}
		if (Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			ledAction(OFF, &ambient_led_flag, AMBIENT_LED);

			ledBlinking( 2, 500, HAZARD_LED, &hazard_led_flag ,GPT_GetElapsedTime() );

			if (GPT_CheckTimeIsElapsed() == 1)
			{
				ledAction(OFF, &hazard_led_flag, HAZARD_LED);
				closing = 0;
			}
		}
	}
}

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

