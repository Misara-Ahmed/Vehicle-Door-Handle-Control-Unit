

#include "Rcc.h"

int main()
{
	Rcc_Init();

	Rcc_Enable(RCC_TIM4);

	Rcc_Enable(RCC_GPIOA);
	
	while(1)
	{

	}
}
