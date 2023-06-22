/**
 * Gpio.c
 *
 *  Created on: Tue Apr 11 2023
 *  Author    : Abdullah Darwish
 */

#include "Gpio.h"

#include "Gpio_Private.h"
#include "Utils.h"

void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode, uint8 DefaultState) {
  switch (PortName) {
    case GPIO_A:
		GPIOA_MODER &= ~(0x03 << (PinNum * 2));
      	GPIOA_MODER |= (PinMode << (PinNum * 2));
		if(PinMode == GPIO_OUTPUT)
		{
			GPIOA_OTYPER &= ~(0x01 << PinNum);
      		GPIOA_OTYPER |= (DefaultState << PinNum);
		}
		else
		{
			GPIOA_PUPDR &= ~(0x03 << (PinNum * 2));
			GPIOA_PUPDR |= (DefaultState << PinNum*2);
		}
      	break;
    case GPIO_B:
      	GPIOB_MODER &= ~(0x03 << (PinNum * 2));
      	GPIOB_MODER |= (PinMode << (PinNum * 2));
		
		if(PinMode == GPIO_OUTPUT)
		{
			GPIOB_OTYPER &= ~(0x01 << PinNum);
      		GPIOB_OTYPER |= (DefaultState << PinNum);
		}
		else
		{
			GPIOB_PUPDR &= ~(0x03 << (PinNum * 2));
			GPIOB_PUPDR |= (DefaultState << PinNum*2);
		}

      	break;
    case GPIO_C:
      // TODO:
    	GPIOC_MODER &= ~(0x03 << (PinNum * 2));
        GPIOC_MODER |= (PinMode << (PinNum * 2));
		if(PinMode == GPIO_OUTPUT)
		{
			GPIOC_OTYPER &= ~(0x01 << PinNum);
      		GPIOC_OTYPER |= (DefaultState << PinNum);
		}
		else
		{
			GPIOC_PUPDR &= ~(0x03 << (PinNum * 2));
			GPIOC_PUPDR |= (DefaultState << PinNum*2);
		}

        break;
    case GPIO_D:
      // TODO:
        GPIOD_MODER &= ~(0x03 << (PinNum * 2));
        GPIOD_MODER |= (PinMode << (PinNum * 2));

		if(PinMode == GPIO_OUTPUT)
		{
			GPIOD_OTYPER &= ~(0x01 << PinNum);
      		GPIOD_OTYPER |= (DefaultState << PinNum);
		}
		else
		{
			GPIOD_PUPDR &= ~(0x03 << (PinNum * 2));
			GPIOD_PUPDR |= (DefaultState << PinNum*2);
		}

        break;
    default:
      break;
  }
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNum, uint8 Data) {

 //TODO: check if the pin is output
  switch (PortName) {
    case GPIO_A:
    	if ( (GPIOA_MODER & (0x03 << (PinNum*2)) ) == (GPIO_OUTPUT << (PinNum*2)) )
    	{
    		GPIOA_ODR &= ~(0x1 << PinNum);
    	    GPIOA_ODR |= (Data << PinNum);
    	    return OK;
    	}
    	else
    	{
    		return NOK;
    	}
    	break;
    case GPIO_B:
    	if ( (GPIOB_MODER & (0x03 << (PinNum*2)) ) == (GPIO_OUTPUT << (PinNum*2)) )
    	{
    		GPIOB_ODR &= ~(0x1 << PinNum);
    	    GPIOB_ODR |= (Data << PinNum);
    	    return OK;
    	}
    	else
    	{
    		return NOK;
    	}
    	break;
    case GPIO_C:
      // TODO:
    	if ( (GPIOC_MODER & (0x03 << (PinNum*2)) ) == (GPIO_OUTPUT << (PinNum*2)) )
    	{
    		GPIOC_ODR &= ~(0x1 << PinNum);
    	    GPIOC_ODR |= (Data << PinNum);
    	    return OK;
    	}
    	else
    	{
    		return NOK;
    	}
      break;
    case GPIO_D:
      // TODO:
    	if ( (GPIOD_MODER & (0x03 << (PinNum*2)) ) == (GPIO_OUTPUT << (PinNum*2)) )
    	{
    		GPIOD_ODR &= ~(0x1 << PinNum);
    	    GPIOD_ODR |= (Data << PinNum);
    	    return OK;
    	}
    	else
    	{
    		return NOK;
    	}
      break;
    default:
      break;
  }
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum)
{
	uint8 Data;
	switch (PortName)
	{
		case GPIO_A:
		if ( (GPIOA_MODER & (0x03 << (PinNum*2)) ) == (GPIO_INPUT << (PinNum*2)) )
		{
			Data = ( (GPIOA_IDR >> PinNum) & 0x01 );
		}
		else
		{
			Data = "Not an input pin";
		}
		break;

		case GPIO_B:
		if ( (GPIOB_MODER & (0x03 << (PinNum*2)) ) == (GPIO_INPUT << (PinNum*2)) )
		{
			Data = ( (GPIOB_IDR >> PinNum) & 0x01 );
		}
		else
		{
			Data = "Not an input pin";
		}
		break;

		case GPIO_C:
		if ( (GPIOC_MODER & (0x03 << (PinNum*2)) ) == (GPIO_INPUT << (PinNum*2)) )
		{
			Data = ( (GPIOC_IDR >> PinNum) & 0x01 );
		}
		else
		{
			Data = "Not an input pin";
		}
		break;

		case GPIO_D:
		if ( (GPIOD_MODER & (0x03 << (PinNum*2)) ) == (GPIO_INPUT << (PinNum*2)) )
		{
			Data = ( (GPIOD_IDR >> PinNum) & 0x01 );
		}
		else
		{
			Data = "Not an input pin";
		}
		break;

		default:
		break;
	}
	return Data;

}
