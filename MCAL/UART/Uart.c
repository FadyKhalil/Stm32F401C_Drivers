/*
 * Uart.c
 *
 *  Created on: Apr 12, 2022
 *      Author: fady
 */


#include <stdlib.h>
#include "../../Services/Std_types.h"
#include "../RCC/Rcc.h"
#include "../NVIC/Nvic.h"
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

typedef struct
{
	u8 Transmit;
	u8 TX_DR_Empty;
	u8 RX_DR_Empty;

}Interrupt_State;

/*************************Define**********************/

/*Register Uart Addresses*/
#define UART1_ADDRESS	((Uart_sRegisters*)0x40011000)
#define UART2_ADDRESS	((Uart_sRegisters*)0x40004400)
#define UART6_ADDRESS	((Uart_sRegisters*)0x40011400)

/***************Static Variable*************************/

static u8 Uart_prvBuzyFlag[3];
static u8 Uart_prvBufferIndex[3];
static u8 Uart_prvBufferSize[3];

static pu8 Uart1_addBufferValue;
static pu8 Uart2_addBufferValue;
static pu8 Uart6_addBufferValue;

static CallBack	cbf[3];

static Interrupt_State Interrupt;

/*receiving*/
static u8 Uart_prvRx_BuzyFlag[3];
static u8 Uart_prvRx_BufferIndex[3];
static u8 Uart_prvRx_BufferSize[3];
static pu8 Uart_prvRx_BufferReceive[3];
static CallBack	cbfRX[3];


/********************Function Definition******************/


void Uart_vidInit(const Uart_ConfigType* ConfigPtr)
{
	u32 Loc_u32TempValue;
	u16	Loc_u16TempMantissa;
	u16 Loc_u16TempFraction;

	if(ConfigPtr == NULL)
	{
		//return null pointer error
	}
	else
	{
		Loc_u32TempValue = ((u64)F_CLK * 1000) / (ConfigPtr->BaudRate * (8 * (2 - ConfigPtr->Oversampling)));
		Loc_u16TempFraction = (Loc_u32TempValue % 1000) * (8 * (2 - ConfigPtr->Oversampling));
		Loc_u16TempFraction = Loc_u16TempFraction / 1000;
		if(Loc_u16TempFraction > 0xF)
		{
			Loc_u16TempMantissa += (Loc_u16TempFraction & 0xF0);
		}
		Loc_u16TempMantissa = Loc_u32TempValue / 1000;

		switch(ConfigPtr->Channel)
		{

		/*
		 * Configuration for
		 *
		 * 	---> UART1
		 * */
		case UART1:

			Rcc_SetAPB2Peripheral(SET_RESET_USART1);

			/*Store the Baudrate*/
			UART1_ADDRESS->BRR = (Loc_u16TempMantissa << 4) | (Loc_u16TempFraction & 0x0F);

			/*Clear Control register for UART*/
			UART1_ADDRESS->CR1 = 0;

			/*enable interrupt*/
			Nvic_vidEnableIrq(NVIC_USART1_INTERRUPT);

			/*configure the UART*/
			UART1_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART1_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART1_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART1_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART1_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			/*configure interrupt pin in UART in Cr1 register to store it in global variables*/
			Interrupt.Transmit = ConfigPtr->TransComplete_Int << 6;
			Interrupt.TX_DR_Empty = ConfigPtr->TransDateRegisterEmpty_Int << 7;
			Interrupt.RX_DR_Empty = ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;

			/*enable UART*/
			UART1_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;

			/*clear the sr register*/
			UART1_ADDRESS->SR = 0;

			break;


		/*
		 * Configuration for
		 *
		 *    --> UART2
		 * */
		case UART2:

			Rcc_SetAPB2Peripheral(SET_RESET_USART2);

			/*Store the Baudrate*/
			UART2_ADDRESS->BRR = (Loc_u16TempMantissa << 4) | (Loc_u16TempFraction & 0x0F);

			/*Clear Control register for UART*/
			UART2_ADDRESS->CR1 = 0;

			/*enable interrupt*/
			Nvic_vidEnableIrq(NVIC_USART1_INTERRUPT);

			/*configure the UART*/
			UART2_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART2_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART2_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART2_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART2_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			/*configure interrupt pin in UART in Cr1 register to store it in global variables*/
			Interrupt.Transmit = ConfigPtr->TransComplete_Int << 6;
			Interrupt.TX_DR_Empty = ConfigPtr->TransDateRegisterEmpty_Int << 7;
			Interrupt.RX_DR_Empty = ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;

			/*enable UART*/
			UART2_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;

			/*clear the sr register*/
			UART2_ADDRESS->SR = 0;

			break;


		/*
		 * Configuration for
		 *
		 * 		--> UART6
		 * */
		case UART6:

			Rcc_SetAPB2Peripheral(SET_RESET_USART6);

			/*Store the Baudrate*/
			UART6_ADDRESS->BRR = (Loc_u16TempMantissa << 4) | (Loc_u16TempFraction & 0x0F);

			/*Clear Control register for UART*/
			UART6_ADDRESS->CR1 = 0;

			/*enable interrupt*/
			Nvic_vidEnableIrq(NVIC_USART1_INTERRUPT);

			/*configure the UART*/
			UART6_ADDRESS->CR1 |= ConfigPtr->Oversampling << 15;
			UART6_ADDRESS->CR1 |= ConfigPtr->WordLength << 12;
			UART6_ADDRESS->CR1 |= ConfigPtr->ParityControl << 10;
			UART6_ADDRESS->CR1 |= ConfigPtr->TransEnable << 3;
			UART6_ADDRESS->CR1 |= ConfigPtr->ReceiveEnable << 2;

			/*configure interrupt pin in UART in Cr1 register to store it in global variables*/
			Interrupt.Transmit = ConfigPtr->TransComplete_Int << 6;
			Interrupt.TX_DR_Empty = ConfigPtr->TransDateRegisterEmpty_Int << 7;
			Interrupt.RX_DR_Empty = ConfigPtr->ReceiveDateRegisterEmpty_Int << 5;

			/*enable UART*/
			UART6_ADDRESS->CR1 |= ConfigPtr->UartEnable << 13;

			/*clear the sr register*/
			UART6_ADDRESS->SR = 0;

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

			Uart_prvBuzyFlag[Channel] = 1;
			Uart_prvBufferIndex[Channel] = 1;
			Uart_prvBufferSize[Channel] = 1;
			UART1_ADDRESS->CR1 |= Interrupt.Transmit;

			break;

		case UART2:

			UART2_ADDRESS->DR = Copy_u8Data;

			Uart_prvBuzyFlag[Channel] = 1;
			Uart_prvBufferIndex[Channel] = 1;
			Uart_prvBufferSize[Channel] = 1;
			UART2_ADDRESS->CR1 |= Interrupt.Transmit;

			break;

		case UART6:

			UART6_ADDRESS->DR = Copy_u8Data;

			Uart_prvBuzyFlag[Channel] = 1;
			Uart_prvBufferIndex[Channel] = 1;
			Uart_prvBufferSize[Channel] = 1;
			UART6_ADDRESS->CR1 |= Interrupt.Transmit;

			break;

		}/*end of switch*/

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
			UART1_ADDRESS->DR  = Copy_sConfigBuffer->Data[0];

			break;

		case UART2:

			Uart2_addBufferValue = Copy_sConfigBuffer->Data;
			UART2_ADDRESS->DR  = Copy_sConfigBuffer->Data[0];

			break;

		case UART6:

			Uart6_addBufferValue = Copy_sConfigBuffer->Data;
			UART6_ADDRESS->DR  = Copy_sConfigBuffer->Data[0];

			break;

		}/*end of switch*/

		Uart_prvBuzyFlag[Copy_sConfigBuffer->Channel] = 1;
		Uart_prvBufferIndex[Copy_sConfigBuffer->Channel] = 1;
		Uart_prvBufferSize[Copy_sConfigBuffer->Channel] = Copy_sConfigBuffer->Size;
		UART1_ADDRESS->CR1 |= Interrupt.Transmit;


	}/*end of else*/

	return Loc_enuErrorStatus;

}/*end of Uart_SendBufferZeroCopy*/


Uart_tenuErrorStatus Uart_RegisterCallBackFunction(Uart_Channel Channel, Uart_Mode Mode, CallBack Add_CallBackFunction)
{
	Uart_tenuErrorStatus Loc_enuErrorStatus = Uart_enuOk;

	if(Add_CallBackFunction)
	{
		if (Mode == SEND)
		{
			cbf[Channel] = Add_CallBackFunction;
		}
		else if (Mode == RECEIVE)
		{
			cbfRX[Channel] = Add_CallBackFunction;
		}
	}/*end of if*/

	else
	{
		Loc_enuErrorStatus = Uart_enuNullPointer;

	}/*end of else*/

	return Loc_enuErrorStatus;

}/*end of Uart_RegisterCallBackFunction*/


Uart_tenuErrorStatus Uart_ReceiveBuffer(const Uart_ConfigReceiveBuffer* ReceiveBuffer)
{
	Uart_tenuErrorStatus Loc_enuErrorStatus = Uart_enuOk;

	if(Uart_prvRx_BuzyFlag[ReceiveBuffer->Channel] == 0)
	{
		/*initizalize the receiving buffer*/
		Uart_prvRx_BufferReceive[UART1] = ReceiveBuffer->Data;
		Uart_prvRx_BufferIndex[UART1] = ReceiveBuffer->Index; /*to be deleted*/
		Uart_prvRx_BufferSize[UART1] = ReceiveBuffer->Size;
		Uart_prvRx_BuzyFlag[UART1] = 1;

		switch(ReceiveBuffer->Channel)
		{

		case UART1:
			/*enable interrupt*/
			UART1_ADDRESS->CR1 |= Interrupt.RX_DR_Empty;

			break;

		case UART2:
			/*enable interrupt*/
			UART2_ADDRESS->CR1 |= Interrupt.RX_DR_Empty;

			break;

		case UART6:
			/*enable interrupt*/
			UART6_ADDRESS->CR1 |= Interrupt.RX_DR_Empty;

			break;
		}
	}
	else
	{
		Loc_enuErrorStatus = Uart_enuBuzy;
	}

	return Loc_enuErrorStatus;
}


Uart_tenuErrorStatus Uart_SendBufferDma(void)
{
	Uart_tenuErrorStatus Loc_enuErroStatus = Uart_enuOk;

	/*enable DMA Transmitting in Uart*/
	UART1_ADDRESS->CR3 |= 1 << 7;


	return Loc_enuErroStatus;
}

Uart_tenuErrorStatus Uart_ReceiveBufferDma(void)
{
	Uart_tenuErrorStatus Loc_enuErroStatus = Uart_enuOk;

	/*enable DMA receiving in Uart*/
	UART1_ADDRESS->CR3 |= 1 << 6;


	return Loc_enuErroStatus;
}

/***********************Handler Function******************************/

void USART1_IRQHandler(void)
{
	if(cbf[UART1])
	{
		/*Read transmitting flag*/
		if((UART1_ADDRESS->SR >> 6) & 0x01)
		{
			if(Uart_prvBufferIndex[UART1] == Uart_prvBufferSize[UART1])
			{
				/*clear Buzy Tx flag*/
				Uart_prvBuzyFlag[UART1] = 0;
				/*clear Tx Buffer Size*/
				Uart_prvBufferSize[UART1] = 0;

				cbf[UART1]();

				/*Clear Tc Flag*/
				UART1_ADDRESS->SR &= ~(1 << 6);

			}/*end of if*/
			else
			{
				UART1_ADDRESS->SR &= ~(1 << 6);
				UART1_ADDRESS->DR = Uart1_addBufferValue[Uart_prvBufferIndex[UART1]];
				Uart_prvBufferIndex[UART1]++;

			}/*end of else*/

		}/*end of if*/
	}

	if(cbfRX[UART1])
	{
		if(Uart_prvRx_BuzyFlag[UART1] == 1 && ((UART1_ADDRESS->SR >> 5) & 0x01))
		{
			Uart_prvRx_BufferReceive[UART1][Uart_prvRx_BufferIndex[UART1]] = UART1_ADDRESS->DR;
			if(Uart_prvRx_BufferSize[UART1] == Uart_prvRx_BufferIndex[UART1])
			{
				UART1_ADDRESS->CR1 &= ~(1 << 5);
				Uart_prvRx_BuzyFlag[UART1] = 0;
				Uart_prvRx_BufferSize[UART1] = 0;
				cbfRX[UART1]();
			}
			Uart_prvRx_BufferIndex[UART1]++;
		}
	}
}/*end of function USART1_IRQHandler*/



void USART2_IRQHandler(void)
{
	if(cbf[UART2])
	{
		if((UART2_ADDRESS->SR >> 6) & 0x01)
		{
			if(Uart_prvBufferIndex[UART2] == Uart_prvBufferSize[UART2])
			{
				/*clear Buzy Tx flag*/
				Uart_prvBuzyFlag[UART2] = 0;
				/*clear Tx Buffer Size*/
				Uart_prvBufferSize[UART2] = 0;

				cbf[UART2]();

				/*Clear Tc Flag*/
				UART2_ADDRESS->SR &= ~(1 << 6);

			}/*end of if*/
			else
			{
				UART2_ADDRESS->SR &= ~(1 << 6);
				UART2_ADDRESS->DR = Uart2_addBufferValue[Uart_prvBufferIndex[UART2]];
				Uart_prvBufferIndex[UART2]++;

			}/*end of else*/

		}/*end of if*/
	}

	if(cbfRX[UART2])
	{
		if(Uart_prvRx_BuzyFlag[UART2] == 1 && ((UART2_ADDRESS->SR >> 5) & 0x01))
		{
			Uart_prvRx_BufferReceive[UART2][Uart_prvRx_BufferIndex[UART2]] = UART2_ADDRESS->DR;
			if(Uart_prvRx_BufferSize[UART2] == Uart_prvRx_BufferIndex[UART2])
			{
				UART2_ADDRESS->CR1 &= ~(1 << 5);
				Uart_prvRx_BuzyFlag[UART2] = 0;
				Uart_prvRx_BufferSize[UART2] = 0;
				cbfRX[UART2]();
			}
			Uart_prvRx_BufferIndex[UART2]++;
		}
	}
}/*end of function USART2_IRQHandler*/

void USART6_IRQHandler(void)
{
	/*to send data*/
	if(cbf[UART6])
	{
		if((UART6_ADDRESS->SR >> 6) & 0x01)
		{
			if(Uart_prvBufferIndex[UART6] == Uart_prvBufferSize[UART6])
			{
				/*clear Buzy Tx flag*/
				Uart_prvBuzyFlag[UART6] = 0;
				/*clear Tx Buffer Size*/
				Uart_prvBufferSize[UART6] = 0;

				cbf[UART6]();

				/*Clear Tc Flag*/
				UART6_ADDRESS->SR &= ~(1 << 6);

			}/*end of if*/
			else
			{
				UART6_ADDRESS->SR &= ~(1 << 6);
				UART6_ADDRESS->DR = Uart6_addBufferValue[Uart_prvBufferIndex[UART6]];
				Uart_prvBufferIndex[UART6]++;

			}/*end of else*/

		}/*end of if*/
	}

	/*for receiving data*/
	if(cbfRX[UART6])
	{
		if(Uart_prvRx_BuzyFlag[UART6] == 1 && ((UART6_ADDRESS->SR >> 5) & 0x01))
		{
			Uart_prvRx_BufferReceive[UART6][Uart_prvRx_BufferIndex[UART6]] = UART6_ADDRESS->DR;
			if(Uart_prvRx_BufferSize[UART6] == Uart_prvRx_BufferIndex[UART6])
			{
				UART6_ADDRESS->CR1 &= ~(1 << 5);
				Uart_prvRx_BuzyFlag[UART6] = 0;
				Uart_prvRx_BufferSize[UART6] = 0;
				cbfRX[UART6]();
			}
			Uart_prvRx_BufferIndex[UART6]++;
		}
	}
}/*end of function USART2_IRQHandler*/


