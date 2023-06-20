#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Bit_Operations.h"
#include "TIM.h"

#define LOCKED 					0
#define UNLOCKED 				1

#define CLOSED 					0
#define OPENED					1

#define DOOR_UNLOCK_BTN				1
#define DOOR_HANDLE_BTN				0

#define VEHICLE_LED				0
#define HAZARD_LED				1
#define AMBIENT_LED				2

#define PRESSED 				0
#define RELEASED				1

/* Initial state */
uint8 Vehicle_Lock = LOCKED;
uint8 Vehicle_Door = CLOSED;
uint8 vehicle_led_flag = 0;
uint8 hazard_led_flag = 0;
uint8 ambient_led_flag = 0;
uint8 closing = 0;
uint8 handle_flag=0;
uint8 no_btn_pressed = 0;

void CheckHandle1(void);
void CheckHandle2(void);
void CheckDoor1(void);
void CheckDoor2(void);
void CheckNoBtnPressed(void);
void NoBtnPressed(void);
void CheckHazardLEd(uint8 state);
void CheckAmbientLEd(uint8 state);
void CheckVechileLEd(uint8 state);
void main(void)
{
	/* Enable clock */
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOB);
	Rcc_Enable(RCC_GPIOC);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_TIM4);
	GPT_Init();

	/* Buttons Configuration */
	Gpio_ConfigPin(GPIO_A,DOOR_UNLOCK_BTN,GPIO_INPUT,GPIO_PULL_UP);
	Gpio_ConfigPin(GPIO_A,DOOR_HANDLE_BTN,GPIO_INPUT,GPIO_PULL_UP);

	/* LEDs Configuration */
	Gpio_ConfigPin(GPIO_B,VEHICLE_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);
	Gpio_ConfigPin(GPIO_B,HAZARD_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);
	Gpio_ConfigPin(GPIO_B,AMBIENT_LED,GPIO_OUTPUT,GPIO_PUSH_PULL);

	while (1)
	{
		if(Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			CheckHandle1();
		}

		if(Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 0)
		{
			if(no_btn_pressed == 0)
			{
				CheckNoBtnPressed();
			}

			if( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED)
			{
				CheckDoor1();
			}
			NoBtnPressed();

		}

		if(Vehicle_Lock == UNLOCKED && Vehicle_Door == OPENED)
		{
			CheckAmbientLEd(0);
			GPT_StartTimer(400);
			while(GPT_CheckTimeIsElapsed() != 1);
			CheckDoor2();
		}
		if(Vehicle_Lock == UNLOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			CheckVechileLEd(1);
			CheckHazardLEd(1);
			if(GPT_CheckTimeIsElapsed() == 1)
			{
				Gpio_WritePin(GPIO_B,AMBIENT_LED,LOW);
				ambient_led_flag = 0;
			}
			CheckHandle2();
		}
		if(Vehicle_Lock == LOCKED && Vehicle_Door == CLOSED && closing == 1)
		{
			CheckAmbientLEd(1);

			if (GPT_GetElapsedTime() < 1000 && GPT_GetElapsedTime() > 0)
			{
				CheckHazardLEd(0);
			}
			if (GPT_GetElapsedTime() < 3000 && GPT_GetElapsedTime() > 2000)
			{
				CheckHazardLEd(0);
			}
			if(GPT_GetElapsedTime() < 2000 && GPT_GetElapsedTime() > 1000)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}
			if(GPT_GetElapsedTime() > 3000)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}
			if(GPT_CheckTimeIsElapsed() == 1)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}

		}

	}
}

void CheckHandle1(void)
{
	if( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED && handle_flag==0)
	{
		handle_flag=1;
		GPT_StartTimer(30);
		while(GPT_CheckTimeIsElapsed() != 1);
		if( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED)
		{
			GPT_StartTimer(10000);
			Vehicle_Lock = UNLOCKED;
		}
	}
	else{
		handle_flag=0;
	}

}

void CheckHandle2(void)
{
	if( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED)
	{
		GPT_StartTimer(30);
		while(GPT_CheckTimeIsElapsed() != 1);
		if( Gpio_ReadPin(GPIO_A,DOOR_HANDLE_BTN) == PRESSED)
		{
			GPT_StartTimer(4000);
			Vehicle_Lock = LOCKED;
			closing = 1;
		}
	}
}

void CheckDoor1(void)
{
	GPT_StartTimer(30);
	while(GPT_CheckTimeIsElapsed() != 1);
	if( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN	) == PRESSED)
	{
		Vehicle_Door = OPENED;
	}
}

void CheckDoor2(void)
{
	if( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN) == PRESSED)
	{
		GPT_StartTimer(30);
		while(GPT_CheckTimeIsElapsed() != 1);
		if( Gpio_ReadPin(GPIO_A,DOOR_UNLOCK_BTN	) == PRESSED)
		{
			GPT_StartTimer(6000);
			Vehicle_Door = CLOSED;
			closing = 1;
		}

	}
}

void CheckNoBtnPressed(void)
{
	if(GPT_CheckTimeIsElapsed() == 1)
	{
		GPT_StartTimer(2000);
		no_btn_pressed = 1;
	}
}

void NoBtnPressed(void)
{
	if(no_btn_pressed == 1)
		{
		    CheckVechileLEd(1);
			CheckAmbientLEd(1);
			if (GPT_GetElapsedTime() < 500 && GPT_GetElapsedTime() > 0)
			{
				CheckHazardLEd(0);
			}
			if (GPT_GetElapsedTime() < 1500 && GPT_GetElapsedTime() > 1000)
			{
				CheckHazardLEd(0);
			}
			if(GPT_GetElapsedTime() < 1000 && GPT_GetElapsedTime() > 500)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}
			if(GPT_GetElapsedTime() > 1500)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}
			if(GPT_CheckTimeIsElapsed() == 1)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				Vehicle_Lock=LOCKED;
				no_btn_pressed=0;
				hazard_led_flag = 0;
			}
		}
		else
		{
			CheckVechileLEd(0);
			if(GPT_GetElapsedTime() < 500 && GPT_GetElapsedTime() > 0 )
			{
				CheckHazardLEd(0);
			}
			else if(GPT_GetElapsedTime() > 500)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = 0;
			}
			if(GPT_GetElapsedTime() < 2000)
			{
				CheckAmbientLEd(0);
			}
			if(GPT_GetElapsedTime() > 2000)
			{
				CheckAmbientLEd(1);
			}
		}
}

void CheckHazardLEd(uint8 state)
{
	if(state==0){
		if (hazard_led_flag == state)
		{
			Gpio_WritePin(GPIO_B,HAZARD_LED,HIGH);
			hazard_led_flag = !state;
		}
		else{
			if (hazard_led_flag == state)
			{
				Gpio_WritePin(GPIO_B,HAZARD_LED,LOW);
				hazard_led_flag = !state;
			}
		}
	}

}

void CheckAmbientLEd(uint8 state)
{
	if(state==0){
		if (ambient_led_flag == 0)
		{
			Gpio_WritePin(GPIO_B,AMBIENT_LED,HIGH);
			ambient_led_flag = 1;
		}
	}
	else{
		if (ambient_led_flag == 1)
		{
			Gpio_WritePin(GPIO_B,AMBIENT_LED,LOW);
			ambient_led_flag = 0;
		}
	}
}

void CheckVechileLEd(uint8 state)
{
	if(state==0){
		if (vehicle_led_flag == 0)
		{
			Gpio_WritePin(GPIO_B,VEHICLE_LED,HIGH);
			vehicle_led_flag = 1;
		}
	}
	else{
		if (vehicle_led_flag == 1)
		{
			Gpio_WritePin(GPIO_B,VEHICLE_LED,LOW);
			vehicle_led_flag = 0;
		}
	}
}
