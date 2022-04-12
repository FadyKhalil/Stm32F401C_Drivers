/*
 * Uart.h
 *
 *  Created on: Apr 12, 2022
 *      Author: fady
 */

#ifndef UART_H_
#define UART_H_



/**********************TypeDef*******************/

/*Uart Channel*/
typedef enum
{
	UART1 = 0,
	UART2,
	UART6

}Uart_Channel;


/*Uart Configuration*/
typedef struct
{
	Uart_Channel Channel;
	u8 Oversampling;
	u8 UartEnable;
	u8 WordLength;
	u8 ParityControl;
	u8 TransDateRegisterEmpty_Int;
	u8 TransComplete_Int;
	u8 ReceiveDateRegisterEmpty_Int;
	u8 TransEnable;
	u8 ReceiveEnable;

}Uart_ConfigType;


/*Uart error Status*/
typedef enum
{
	Uart_enuOk = 0,
	Uart_enuNullPointer,
	Uart_enuValueExceed,
	Uart_enuBuzy


}Uart_tenuErrorStatus;


/*Uart Data storage*/
typedef struct
{
	pu8 Data;
	u32 Size;
	Uart_Channel Channel;

}Uart_ConfigBuffer;

/*callback function*/
typedef void(*CallBack)(void);



/***************Function Declaration*******************/
void Uart_vidGenerateBaudRate(u16 Mantissa, u8 Fraction);


void Uart_vidInit(const Uart_ConfigType* ConfigPtr);


Uart_tenuErrorStatus Uart_SendByteAsynchronous(Uart_Channel Channel, u8 Copy_u8Data);


Uart_tenuErrorStatus Uart_RegisterCallBackFunction(Uart_Channel Channel, CallBack Add_CallBackFunction);


Uart_tenuErrorStatus Uart_SendBufferZeroCopy(Uart_Channel Channel, pu8 Add_pu8Buffer, u32 Copy_u32DataSize);





#endif /* UART_H_ */
