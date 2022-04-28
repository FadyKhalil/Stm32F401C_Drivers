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

enum
{
	Uart_DISABLE = 0,
	Uart_ENABLE
};


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
	Uart_Channel Channel;
	pu8 Data;
	u32 Size;
	u32 Index;

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

/*
 * GPIO Module
 *
 * ======================= GPIO_tstrPinConfiguration ===================
 * --> Configure the pin 10 as alternative function (Gpio_Mode = GPIO_u8ALTFUNC_PUSHPULL)
 * --> Select the alternative as Uart in case of Uart1 (GPIO_ALTF = GPIO_ALTERNATIVE_USART1)
 *
*/

/*
 * Function Name: 		Uart_vidInit
 *
 *
 * Description:			This function to Configure the Uart
 *
 *
 * Parameter:			const Uart_ConfigType* ConfigPtr
 * 						(there is structure created for initialization)
 * 						(to Initialize each mode required)
 *
 *
 * return:				Error Status
 *
 *
 * */
extern void Uart_vidInit(const Uart_ConfigType* ConfigPtr);

/*
 * Function Name: 		Uart_SendByteAsynchronous
 *
 *
 * Description:			This function to Send byte Asynchrounous
 *
 *
 * Parameter:			Uart_Channel Channel --> to chose the Channel
 * 												in range {UART1, UART2, UART6}
 *
 * 						Copy_u8Data --> Data to be sent
 *
 *
 * return:				Error Status
 *
 *
 * */
extern Uart_tenuErrorStatus Uart_SendByteAsynchronous(Uart_Channel Channel, u8 Copy_u8Data);

/*
 * Function Name: 		Uart_ReceiveBuffer
 *
 *
 * Description:			To Receive Buffer
 *
 *
 * Parameter:			const Uart_ConfigReceiveBuffer* ReceiveBuffer
 * 						(there is structure created for initialization)
 * 						to Initialize Channel , Data (the address of reception buffer)
 * 						Size (of the receiving buffer)
 *
 *
 * return:				Error Status
 *
 *
 * */
extern Uart_tenuErrorStatus Uart_ReceiveBuffer(const Uart_ConfigReceiveBuffer* ReceiveBuffer);

/*
 * Function Name: 		Uart_SendBufferZeroCopy
 *
 *
 * Description:			To Receive Buffer
 *
 *
 * Parameter:			const Uart_ConfigBuffer* Copy_sConfigBuffer
 * 						(there is structure created for initialization)
 * 						to Initialize Channel , Data (the address of Transmitting buffer)
 * 						Size (of the receiving buffer)
 *
 *
 * return:				Error Status
 *
 *
 * */
extern Uart_tenuErrorStatus Uart_SendBufferZeroCopy(const Uart_ConfigBuffer* Copy_sConfigBuffer);

/*
 * Function Name: 		Uart_RegisterCallBackFunction
 *
 *
 * Description:			This function to Send byte Asynchrounous
 *
 *
 * Parameter:			Uart_Channel Channel --> to chose the Channel
 * 												in range {UART1, UART2, UART6}
 *
 * 						Mode --> in range {SEND, RECEIVE}
 *
 *
 * 						Add_CallBackFunction --> Register the call function
 *
 *
 * return:				Error Status
 *
 *
 * */
extern Uart_tenuErrorStatus Uart_RegisterCallBackFunction(Uart_Channel Channel, Uart_Mode Mode, CallBack Add_CallBackFunction);





/////////////////////////////////////////////////////////////////////////////////////
/*
 * How to configure UART using DMA
 *
 *  					-------------
 * 						| Receiving |
 * 						-------------
 * Uart Module
 * -------------
 *
 * ========================== Uart_ConfigType ======================
 * --> enable the receiving bit (ReceiveEnable) in the Uart configuration
 * --> enable the Uart
 * --> Disable all interrupt in case no need for any parity or idle checking
 * --> Initialize your baudrate
 *
 *
 * Dma Module
 * -------------
 *
 * =======================  Dma_config_t ==========================
 * --> Enable Stream 5 (for RX UART1)
 * --> Enable Channel 4 (for RX UART1)
 * --> Initialize whether you need it circular or not (used with double buffering if you want too)
 * --> Data direction to Peripheral to memory  {Data_Direction = Per_to_Mem}
 * --> Memory to be auto increment
 * --> Peripheral disable auto increment (as you only need to focus on DR register)
 * --> Initialize the memory size to be 1 byte in case of using 8 bit
 * --> Initialize the pheripheral size to be 1 byte as well
 * --> Initialize the priority level required
 * --> enable the interrupt required as example (Interrupt.Transfer_Complete = Dma_Enable)
 *
 *
 * ======================== Criteria_t =========================
 *
 * --> Initialize the Stream 5
 * --> Initialize the address of your buffer (Memory_0_Address = (u32)MemoryDestination)
 * --> Initialize the address of your DR Register of the Uart used in our
 * 		example is UART1 (Peripheral_Address = 0x40011004)
 *
 * Initialization
 * ---------------
 *
 * --> Enable Clocks
 * --> Enable DMA interrupt from NVIC
 * --> Register the call back function
 *
 * ======> Use Uart_ReceiveBufferDma --> to start Receiving
 * How to configure UART using DMA
 *
 *
 *
 *
 *
 *  					----------------
 * 						| Transmitting |
 * 						----------------
 * Uart Module
 * -------------
 *
 * ========================== Uart_ConfigType ======================
 * --> enable the Transmitting bit (TransmitEnable) in the Uart configuration
 * --> enable the Uart
 * --> Disable all interrupt in case no need for any parity or idle checking
 * --> Initialize your baudrate
 *
 *
 * Dma Module
 * -------------
 *
 * =======================  Dma_config_t ==========================
 * --> Enable Stream 7 (for TX UART1)
 * --> Enable Channel 4 (for TX UART1)
 * --> Initialize whether you need it circular or not (used with double buffering if you want too)
 * --> Data direction to Memory to Peripheral  {Data_Direction = Mem_to_Per}
 * --> Memory to be auto increment
 * --> Peripheral disable auto increment (as you only need to focus on DR register)
 * --> Initialize the memory size to be 1 byte in case of using 8 bit
 * --> Initialize the pheripheral size to be 1 byte as well
 * --> Initialize the priority level required
 * --> enable the interrupt required as example (Interrupt.Transfer_Complete = Dma_Enable)
 *
 *
 * ======================== Criteria_t =========================
 *
 * --> Initialize the Stream 7
 * --> Initialize the address of your buffer (Memory_0_Address = (u32)MemoryDestination)
 * --> Initialize the address of your DR Register of the Uart used in our
 * 		example is UART1 (Peripheral_Address = 0x40011004)
 *
 * Initialization
 * ---------------
 *
 * --> Enable Clocks
 * --> Enable DMA interrupt from NVIC
 * --> Register the call back function
 *
 * ======> Use Uart_ReceiveBufferDma --> to start Receiving
 *
 * */



/*
 * Function Name: 		Uart_SendBufferDma
 *
 *
 * Description:			This function to Receive data using DMA
 *
 *
 * Parameter:			void
 *
 *
 * return:				Error Status
 *
 *
 * */
extern Uart_tenuErrorStatus Uart_ReceiveBufferDma(void);


 /*
  * Function Name: 		Uart_SendBufferDma
  *
  *
  * Description:		This function to send data using DMA
  *
  *
  * Parameter:			void
  *
  *
  * return:				Error Status
  *
  *
  * */
extern Uart_tenuErrorStatus Uart_SendBufferDma(void);






#endif /* UART_H_ */
