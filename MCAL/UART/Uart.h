/*
 * Uart.h
 *
 *  Created on: Apr 12, 2022
 *      Author: fady
 */

#ifndef UART_H_
#define UART_H_



/**********************TypeDef*******************/

#define F_CLK	16000000


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
	u16 BaudRate;

}Uart_ConfigType;


/*Uart error Status*/
typedef enum
{
	Uart_enuOk = 0,
	Uart_enuNullPointer,
	Uart_enuValueExceed,
	Uart_enuBuzy


}Uart_tenuErrorStatus;


/*Uart Data storage to send*/
typedef struct
{
	pu8 Data;
	u32 Size;
	Uart_Channel Channel;

}Uart_ConfigBuffer;

/*Uart Data storage to Receive*/
typedef struct
{
	pu8 Data;
	u32 Size;
	u32 Index;
	Uart_Channel Channel;

}Uart_ConfigReceiveBuffer;

/*Uart Mode*/
typedef enum
{
	SEND,
	RECEIVE

}Uart_Mode;



/*callback function*/
typedef void(*CallBack)(void);



/************************DEFINE***********************/

#define ENABLE 		1
#define DISABLE		0

/***************Function Declaration*******************/


void Uart_vidInit(const Uart_ConfigType* ConfigPtr);


Uart_tenuErrorStatus Uart_SendByteAsynchronous(Uart_Channel Channel, u8 Copy_u8Data);

Uart_tenuErrorStatus Uart_ReceiveBuffer(const Uart_ConfigReceiveBuffer* ReceiveBuffer);

Uart_tenuErrorStatus Uart_RegisterCallBackFunction(Uart_Channel Channel, Uart_Mode Mode, CallBack Add_CallBackFunction);

Uart_tenuErrorStatus Uart_SendBufferZeroCopy(const Uart_ConfigBuffer* Copy_sConfigBuffer);





#endif /* UART_H_ */
