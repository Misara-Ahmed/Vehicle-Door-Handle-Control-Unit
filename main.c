#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "keypad.h"
#include "Bit_Operations.h"
#include "stm32f401xe.h"
#include "Gpio_private.h"
#include "TIM.h"
typedef struct {
	uint32 IMR;
	uint32 EMR;
	uint32 RTSR;
	uint32 FTSR;
	uint32 SWIER;
	uint32 PR;
} ExtiType;

typedef struct{
	uint32 SYSCFG_MEMRMP;
	uint32 SYSCFG_PMC;
	uint32 SYSCFG_EXTICR1;
	uint32 SYSCFG_EXTICR2;
	uint32 SYSCFG_EXTICR3;
	uint32 SYSCFG_EXTICR4;
	uint32 SYSCFG_CMPCR;
}SYSCFG_EXTICR;

#define NVIC_ISER0 (*(uint32 *)0xE000E100)

#define EXTI ((ExtiType *)0x40013C00)
#define SYSCFG_EXTI ((SYSCFG_EXTICR *)0x40013800)

sint8 Seven_seg_counter = 0;

void Exti_Enable(uint8 line_num)
{
	SET_BIT(EXTI->IMR, line_num);  // enable interrupt on this line
}

void Exti_Disable(uint8 line_num)
{
	CLEAR_BIT(EXTI->IMR, line_num);  // disable interrupt on this line
}

typedef struct{
	uint8 line_num;          // enable line
	uint8 EXTI_NVIC_LineNum; // enable nvic_line which start from 6
	uint8 edgeType;			 // choose falling edge or raising edge
	uint8 Mcu_pin_num;		// choose which pin in MicroController
	uint8 PortName;
}EXT_config;


//void Exti_Init(EXT_config * ext_config)
//{
//	/* */
//	Exti_Enable(ext_config->line_num);
//
//	SET_BIT(NVIC_ISER0, ext_config->EXTI_NVIC_LineNum);
//
//	if (ext_config->edgeType == 0)
//	{
//		SET_BIT(EXTI->RTSR, ext_config->line_num);
//	}
//	else if (ext_config->edgeType == 1)
//	{
//		SET_BIT(EXTI->FTSR, ext_config->line_num);
//	}
//	else
//	{
//		/* do nothing */
//	}
//
//	Gpio_ConfigPin(ext_config->PortName, ext_config->Mcu_pin_num, GPIO_INPUT, GPIO_PULL_UP);
//	//Gpio_ConfigPin(GPIO_B, ext_config->Mcu_pin_num, GPIO_INPUT, GPIO_PULL_UP);
//	//Gpio_ConfigPin(GPIO_C, ext_config->Mcu_pin_num, GPIO_INPUT, GPIO_PULL_UP);
//	Gpio_ConfigPin(ext_config->PortName, ext_config->Mcu_pin_num, GPIO_INPUT, GPIO_PULL_UP);
//
//
//	/* port D : 0011*/
//	SET_BIT(SYSCFG_EXTI->SYSCFG_EXTICR1,0);
//	SET_BIT(SYSCFG_EXTI->SYSCFG_EXTICR1,1);
//	CLEAR_BIT(SYSCFG_EXTI->SYSCFG_EXTICR1,2);
//	CLEAR_BIT(SYSCFG_EXTI->SYSCFG_EXTICR1,3);
//
//}
/* interrupt line 0 config :
 * Enable interrupt line for EXIT = 0,
 * EXTI_NVIC_LineNum = 6,
 * edgeType = 1-->(Falling edge), 0-->(Raising edge)
 * Mcu_pin_num = 0
 * port
 * */
//EXT_config Ext0_configurations = {0,6,Falling_edge,0,GPIO_A};
//EXT_config Ext1_configurations = {1,7,Falling_edge,1,GPIO_D};


void main(void) {

	/*Enable clk for ports */
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOB);
	Rcc_Enable(RCC_GPIOC);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_TIM4);
	GPT_Init();
//	Rcc_Enable(RCC_SYSCFG);

//	Exti_Init(&Ext0_configurations);
//	Exti_Init(&Ext1_configurations);

	/*Buttons*/
	Gpio_ConfigPin(GPIO_A,0,GPIO_INPUT,GPIO_PULL_UP); // Door button
	Gpio_ConfigPin(GPIO_A,1,GPIO_INPUT,GPIO_PULL_UP); // handel_button


	/*Leds*/
	Gpio_ConfigPin(GPIO_B,0,GPIO_OUTPUT,GPIO_PUSH_PULL); // Vechile led
	Gpio_ConfigPin(GPIO_B,1,GPIO_OUTPUT,GPIO_PUSH_PULL); // Hazard Led
	Gpio_ConfigPin(GPIO_B,2,GPIO_OUTPUT,GPIO_PUSH_PULL); // Ambient led
	uint8 Vechile_Lock=Locked;
	uint8 Vechile_Door=Closed;

	/*Configuration Pins For 7 Segment  */
//	for (uint32 counter = 0; counter < 7; counter++) {
//		Gpio_ConfigPin(GPIO_B, counter, GPIO_OUTPUT, GPIO_PUSH_PULL);
//
//	}


	while (1) {
		/* do nothing */

		if(Gpio_ReadPin(GPIO_A,1)==0){
			GPT_StartTimer(30);
			while(GPT_CheckTimeIsElapsed()!=1);
			if(Gpio_ReadPin(GPIO_A,1)==0){
				Vechile_Lock=UnLocked;
				Gpio_WritePin(GPIO_B,0,HIGH);
				GPT_StartTimer(2000);

				while(GPT_CheckTimeIsElapsed()!=1){
					Gpio_WritePin(GPIO_B,2,HIGH);
						while(GPT_CheckTimeIsElapsed()<=500){
							Gpio_WritePin(GPIO_B,1,HIGH);
						}
						Gpio_WritePin(GPIO_B,1,LOW);
					}
					Gpio_WritePin(GPIO_B,2,LOW);
				}

//			if(Gpio_ReadPin(GPIO_A,0)==0){
//				GPT_StartTimer(30);
//				while(GPT_CheckTimeIsElapsed()!=1);
//				if(Gpio_ReadPin(GPIO_A,0)==0){
//					Vechile_Door=Open;
//					Gpio_WritePin(GPIO_B,2,HIGH);
//				}
//
//			}
//			if(Gpio_ReadPin(GPIO_A,0)==0){
//				GPT_StartTimer(30);
//				while(GPT_CheckTimeIsElapsed()!=1);
//				if(Gpio_ReadPin(GPIO_A,0)==0){
//					GPT_StartTimer(1000);
//					Vechile_Door=Closed;
//					Gpio_WritePin(GPIO_B,0,LOW);
//					Gpio_WritePin(GPIO_B,1,LOW);
//					while(GPT_CheckTimeIsElapsed()!=1){
//						Gpio_WritePin(GPIO_B,2,HIGH);
//						}
//						Gpio_WritePin(GPIO_B,2,LOW);
//
//				}
//			}
//			if(Gpio_ReadPin(GPIO_A,1)==0){
//				GPT_StartTimer(30);
//				while(GPT_CheckTimeIsElapsed()!=1);
//				if(Gpio_ReadPin(GPIO_A,1)==0){
//					Vechile_Door=Closed;
//					Gpio_WritePin(GPIO_B,0,LOW);
//					GPT_StartTimer(2000);
//					while(GPT_CheckTimeIsElapsed()!=1){
//						Gpio_WritePin(GPIO_B,2,LOW);
//						while(GPT_CheckTimeIsElapsed()<=500){
//							Gpio_WritePin(GPIO_B,1,HIGH);
//						}
//						while(GPT_CheckTimeIsElapsed()<=1000){
//							Gpio_WritePin(GPIO_B,1,LOW);
//						}
//						while(GPT_CheckTimeIsElapsed()<=1500){
//							Gpio_WritePin(GPIO_B,1,HIGH);
//						}
//						while(GPT_CheckTimeIsElapsed()<=2000){
//							Gpio_WritePin(GPIO_B,1,LOW);
//						}
//
//
//						}
//					}
//				}


		}
	}
}



//void EXTI0_IRQHandler(void)
//{
//	Exti_Disable(1);
//	Exti_Disable(2);
//	Exti_Disable(3);
//	Exti_Disable(4);
//
//	Seven_seg_counter++;
//	increament(Seven_seg_counter);
//
//	if(Seven_seg_counter > 9)
//	{
//		Seven_seg_counter = 0;
//	}
//	/* clear pending flag */
//	SET_BIT(EXTI->PR, 0);
//	/*Race conditions*/
//	Exti_Enable(1);
//	Exti_Enable(2);
//	Exti_Enable(3);
//	Exti_Enable(4);
//
//}
//
//void EXTI1_IRQHandler(void)
//{
//	Exti_Disable(0);
//	Exti_Disable(2);
//	Exti_Disable(3);
//	Exti_Disable(4);
//	Seven_seg_counter--;
//	decrament(Seven_seg_counter);
//
//	if(Seven_seg_counter == 0)
//		{
//			Seven_seg_counter = 10;
//		}
//	/* clear pending flag */
//	SET_BIT(EXTI->PR, 1);
//	/*Race conditions*/
//	Exti_Enable(0);
//	Exti_Enable(2);
//	Exti_Enable(3);
//	Exti_Enable(4);
//}

