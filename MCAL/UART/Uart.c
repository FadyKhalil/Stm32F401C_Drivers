/*
 * Uart.c
 *
 *  Created on: Apr 12, 2022
 *      Author: fady
 */


#include <stdlib.h>
#include "Services/Std_types.h"
#include "MCAL/Rcc.h"
#include "MCAL/Nvic.h"
#include "Uart.h"

/*****************typedef**************************/

typedef struct
{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 CR4;

}Uart_sRegisters;

/*************************Define**********************/

/*Register Uart Addresses*/
#define UART1_ADDRESS	((Uart_sRegisters*)0x40011000)
#define UART2_ADDRESS	((Uart_sRegisters*)0x40004400)
#define UART6_ADDRESS	((Uart_sRegisters*)0x40011400)

/***************Static Variable*************************/

static u16 Uart_prvBaudRate;
static u8 Uart_prvBuzyFlag[3];
static u8 Uart_prvBufferIndex[3];
static u8 Uart_prvBufferSize[3];
static pu8 Uart1_addBufferValue;
static pu8 Uart2_addBufferValue;
static pu8 Uart6_addBufferValue;
static CallBack	cbf[3];


/********************Function Definition******************/


void Uart_vidGenerateBaudRate(u16 Mantissa, u8 Fraction)
{
	if(Mantissa > 0x0FFF || Fraction > 0x0F)
	{
		//Return Error Value
	}

	else
	{
		Uart_prvBaudRate = (Mantissa << 4) | Fraction;
	}

	return;
}


void Uart_vidInit(const Uart_ConfigType* ConfigPtr)
{

	if(ConfigPtr == NULL)
	{
		//return null pointer error
	}
	else
	{
		switch(ConfigPtr->Channel)
		{
		case UART1:

			Rcc_SetAPB2Peripheral(SET_RESET_USART1);

			UART1_ADDRESS->CR1 = 0;

			UART1_ADDRESS->BRR = Uart_prvBaudRate;

			UART1_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART1_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;
			UART1_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART1_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART1_ADDRESS->CR1 |= ConfigPtr->TransDateRegisterEmpty_Int << 7;
			UART1_ADDRESS->CR1 |= ConfigPtr->TransComplete_Int << 6;
			UART1_ADDRESS->CR1 |= ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;
			UART1_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART1_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			Nvic_vidEnableIrq(NVIC_USART1_INTERRUPT);

			break;

		case UART2:

			Rcc_SetAPB2Peripheral(SET_RESET_USART2);

			UART2_ADDRESS->CR1 = 0;

			UART2_ADDRESS->BRR = Uart_prvBaudRate;

			UART2_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART2_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;
			UART2_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART2_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART2_ADDRESS->CR1 |= ConfigPtr->TransDateRegisterEmpty_Int << 7;
			UART2_ADDRESS->CR1 |= ConfigPtr->TransComplete_Int << 6;
			UART2_ADDRESS->CR1 |= ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;
			UART2_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART2_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			Nvic_vidEnableIrq(NVIC_USART2_INTERRUPT);

			break;

		case UART6:

			Rcc_SetAPB2Peripheral(SET_RESET_USART6);

			UART6_ADDRESS->CR1 = 0;

			UART6_ADDRESS->BRR = Uart_prvBaudRate;

			UART6_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART6_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;
			UART6_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART6_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART6_ADDRESS->CR1 |= ConfigPtr->TransDateRegisterEmpty_Int << 7;
			UART6_ADDRESS->CR1 |= ConfigPtr->TransComplete_Int << 6;
			UART6_ADDRESS->CR1 |= ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;
			UART6_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART6_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			Nvic_vidEnableIrq(NVIC_USART6_INTERRUPT);

			break;

		}/*end of switch*/

	}/*end of else*/

}/*end of function Uart_vidInit*/



Uart_tenuErrorStatus Uart_SendByteAsynchronous(Uart_Channel Channel, u8 Copy_u8Data)
{
	Uart_tenuErrorStatus Loc_enuErrorStatus = Uart_enuOk;

	if(Uart_prvBuzyFlag[Channel] == 1)
	{
		Loc_enuErrorStatus = Uart_enuBuzy;

	}/*end of if*/
	else
	{
		switch(Channel)
		{
		case UART1:

			UART1_ADDRESS->DR = Copy_u8Data;

			break;

		case UART2:

			UART2_ADDRESS->DR = Copy_u8Data;

			break;

		case UART6:

			UART6_ADDRESS->DR = Copy_u8Data;

			break;

		}/*end of switch*/

		Uart_prvBuzyFlag[Channel] = 1;
		Uart_prvBufferIndex[Channel] = 1;
		Uart_prvBufferSize[Channel] = 1;

	}/*end of else*/

	return Loc_enuErrorStatus;

}/*end of if*/



Uart_tenuErrorStatus Uart_SendBufferZeroCopy(const Uart_ConfigBuffer* Copy_sConfigBuffer)
{
	Uart_tenuErrorStatus Loc_enuErrorStatus = Uart_enuOk;

	if(Copy_sConfigBuffer == NULL)
	{
		Loc_enuErrorStatus = Uart_enuNullPointer;

	}/*end of if*/

	else if(Uart_prvBuzyFlag[Copy_sConfigBuffer->Channel] == 1)
	{
		Loc_enuErrorStatus = Uart_enuBuzy;

	}/*end of else if*/

	else
	{
		switch(Copy_sConfigBuffer->Channel)
		{
		case UART1:

			Uart1_addBufferValue = Copy_sConfigBuffer->Data;

			break;

		case UART2:

			Uart2_addBufferValue = Copy_sConfigBuffer->Data;

			break;

		case UART6:

			Uart6_addBufferValue = Copy_sConfigBuffer->Data;

			break;

		}/*end of switch*/

		Uart_prvBuzyFlag[Copy_sConfigBuffer->Channel] = 1;
		Uart_prvBufferIndex[Copy_sConfigBuffer->Channel] = 1;
		Uart_prvBufferSize[Copy_sConfigBuffer->Channel] = Copy_sConfigBuffer->Size;

	}/*end of else*/

	return Loc_enuErrorStatus;

}/*end of Uart_SendBufferZeroCopy*/


Uart_tenuErrorStatus Uart_RegisterCallBackFunction(Uart_Channel Channel, CallBack Add_CallBackFunction)
{
	Uart_tenuErrorStatus Loc_enuErrorStatus = Uart_enuOk;

	if(Add_CallBackFunction)
	{

		cbf[Channel] = Add_CallBackFunction;

	}/*end of if*/

	else
	{
		Loc_enuErrorStatus = Uart_enuNullPointer;

	}/*end of else*/

	return Loc_enuErrorStatus;

}/*end of Uart_RegisterCallBackFunction*/


/***********************Handler Function******************************/

void USART1_IRQHandler(void)
{
	if((UART1_ADDRESS->SR >> 6) & 0x01)
	{
		if(Uart_prvBufferIndex[UART1] == Uart_prvBufferSize[UART1])
		{
			Uart_prvBuzyFlag[UART1] = 0;
			Uart_prvBufferSize[UART1] = 0;

			cbf[UART1]();

		}/*end of if*/
		else
		{
			UART1_ADDRESS->DR = Uart1_addBufferValue[Uart_prvBufferIndex[UART1]];
			Uart_prvBufferIndex[UART1]++;

		}/*end of else*/

	}/*end of if*/

}/*end of function USART1_IRQHandler*/

void USART2_IRQHandler(void)
{
	if((UART2_ADDRESS->SR >> 6) & 0x01)
	{
		if(Uart_prvBufferIndex[UART2] == Uart_prvBufferSize[UART2])
		{
			Uart_prvBuzyFlag[UART2] = 0;
			Uart_prvBufferSize[UART2] = 0;

			cbf[UART2]();

		}/*end of if*/
		else
		{
			UART2_ADDRESS->DR = Uart2_addBufferValue[Uart_prvBufferIndex[UART2]];
			Uart_prvBufferIndex[UART2]++;

		}/*end of else*/

	}/*end of if*/

}/*end of function USART2_IRQHandler*/

void USART6_IRQHandler(void)
{
	if((UART6_ADDRESS->SR >> 6) & 0x01)
	{
		if(Uart_prvBufferIndex[UART6] == Uart_prvBufferSize[UART6])
		{
			Uart_prvBuzyFlag[UART6] = 0;
			Uart_prvBufferSize[UART6] = 0;

			cbf[UART6]();

		}/*end of if*/
		else
		{
			UART6_ADDRESS->DR = Uart6_addBufferValue[Uart_prvBufferIndex[UART6]];
			Uart_prvBufferIndex[UART6]++;

		}/*end of else*/

	}/*end of if*/

}/*end of function USART2_IRQHandler*/


