/*
 * Dma.h
 *
 *  Created on: Apr 21, 2022
 *      Author: fady
 */

#ifndef DMA_H_
#define DMA_H_


typedef enum
{
	Dma_Stream_En = 0,
	Dma_Stream_Off,
	Dma_enu_Undifined_Value,
	Dma_enuNullPtr,
	Dma_enuOk

}Dma_tenuErrorStatus;

/*Call function back function data type*/
typedef void(*Dma_CallFunction)(void);



typedef enum
{
	DMA1 = 0,
	DMA2

}Dma_Id;
/*
 * User Define Type to checked well before configuring the DMA
 *
 * (Dma_Config_t)
 *
 * */
typedef enum
{
	Dma_Stream0 = 0,
	Dma_Stream1 = 1,
	Dma_Stream2 = 2,
	Dma_Stream3 = 3,
	Dma_Stream4 = 4,
	Dma_Stream5 = 5,
	Dma_Stream6 = 6,
	Dma_Stream7 = 7

}StreamId;

typedef enum
{
	Dma_Channel0 = 0,
	Dma_Channel1 = 1,
	Dma_Channel2 = 2,
	Dma_Channel3 = 3,
	Dma_Channel4 = 4,
	Dma_Channel5 = 5,
	Dma_Channel6 = 6,
	Dma_Channel7 = 7

}ChannelId;

typedef enum
{
	Dma_Low = 0,
	Dma_Medium,
	Dma_High,
	Dma_VeryHigh

}Priority_t;

typedef enum
{
	Byte = 0,
	Half_Word,
	Word,

}Size_t;

typedef enum
{
	Per_to_Mem = 0,
	Mem_to_Per,
	Mem_to_Mem

}Direction_t;

typedef enum{

	Dma_DISABLE = 0,
	Dma_ENABLE

}State;

typedef enum
{
	Single = 0,
	Inc_4_Beats,
	Inc_8_Beats,
	Inc_16_Beats

}Burst_t;

/*
 * The struct is made to configure the DMA Channel
 *
 * !! Check all user define types (enum) above for each Mode as use from it
 *
 * !! TO USE IN FUCNTION :: Dma_enuConfiguration
 *
 * */
typedef struct
{
	StreamId Stream;
	ChannelId Channel;
	Burst_t Memory_Burst;
	Burst_t Peripheral_Burst;
	State Double_Buffer;
	Priority_t Priority_Cfg;
	Size_t	Memory_Size;
	Size_t	Peripheral_Size;
	State Mem_Auto_Increment;
	State Per_Auto_Increment;
	State Circular_Mode;
	Direction_t Data_Direction;

	struct
	{
		State Direct_Mode;
		State Transfer_Err;
		State HalfTransfer;
		State Transfer_Complete;

	}Interrupt;

}Dma_config_t;

/*
 * structure to initialize Channel criterial
 *
 * !! TO USE IN FUNCTION :: Dma_enuCritariaChannel
 *
 * */
typedef struct
{
	StreamId Stream;
	u32 Peripheral_Address;
	u32 Memory_0_Address;
	u32 Memory_1_Address;
	u16 Block_Length;

}Criteria_t;


/**************Function Declaration**********************/

/*
 * Function Name:	Dma_vidConfiguration
 *
 * Description:		To Configure Dma Channel
 *
 * Parameter:		ConfigPtr (Pointer) to Dma_config_t (User DataType)
 *
 * return:			ErrorStatus
 * 					in range {---Dma_enuNullPtr---}
 *
 * */
extern Dma_tenuErrorStatus Dma_enuConfiguration(Dma_Id Dma, const Dma_config_t* ConfigPtr);

/*
 * Function Name:	Dma_enuCritariaChannel
 *
 * Description:		To Configure the criteria Channel
 *						--> To initialize the source base address
 *						--> To initialize the destination base address
 *						--> to initialize the Block Length
 *
 * Parameter:		CriteriaPtr (Pointer) to Criteria_t (User DataType)
 *
 *
 * return:			ErrorStatus
 * 					in range {---Dma_enuNullPtr---}
 *
 * */
extern Dma_tenuErrorStatus Dma_enuCritariaChannel(Dma_Id Dma, const Criteria_t* CriteriaPtr);


/*
 * Function Name:	Dma_enuRegisterCallFunction
 *
 * Description:		To Initialize the Call Back Function
 *
 *
 * Parameter:		Channel --> in Range{ Channel0, Channel1, Channel2, Channel3, Channel4, Channel5, Channel6, Channel7 }
 * 					Cbf		--> Register the call back function
 *
 *
 * return:			ErrorStatus
 * 					in range {---Dma_enuNullPtr---}
 **/
extern Dma_tenuErrorStatus Dma_enuRegisterCallFunction(Dma_Id Dma, StreamId Stream, Dma_CallFunction Cbf);


/*
 * Function Name:	Dma_vidStreamState
 *
 * Description:		To open stream on Dma Channel
 *
 * 				!!!	THIS SHOULD BE SET AS LAST THING AFTER YOU FINISH CONFIGURATION
 * 					OR ALL YOUR WRITE WILL BE NEGLECTED --> IN CASE OF ENABLE
 *
 * 				!!! TO RECONFIGURE THE CHANNEL USER MUST SET THE STREAM TO OFF
 *
 *
 * Parameter:		Channel --> in Range{ Channel0, Channel1, Channel2, Channel3, Channel4, Channel5, Channel6, Channel7 }
 * 					Stream --> to configure the state
 *
 * return:			ErrorStatus
 * 					in range {--Variable undifined--}
 *
 * */
extern Dma_tenuErrorStatus Dma_enuStreamState(Dma_Id Dma, StreamId Stream, State Condition);



#endif /* DMA_H_ */
