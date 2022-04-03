/*
 * Systick.h
 *
 *  Created on: Mar 28, 2022
 *      Author: fady
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_


typedef enum
{
	Systick_enuOK = 0,
	Systick_enuWrongValue

}Systick_tenuErrorStatus;

typedef void (*systickcfg_t)(void);


#define CLOCK_FREQUENCY		25000000


#define SYSTICK_PROCESSORCLK_INTERUPT	0x1006
#define SYSTICK_AHB_INTERUPT			0x1002
#define SYSTICK_PROCESSORCLK			0x1004
#define SYSTICK_AHB						0x1000



/*
 * Public Function: 	Systick_vidInit
 *
 *
 * Description:			This function to init the systick
 *
 *
 * Input Parameter:		--> Copy_u16Init
 * 						in range: 	@	SYSTICK_PROCESSORCLK_INTERUPT
 * 								    @	SYSTICK_AHB_INTERUPT
 * 									@	SYSTICK_PROCESSORCLK
* 									@	SYSTICK_AHB
 *
 *
 * Return:				Error Status
 *
 * */
Systick_tenuErrorStatus Systick_vidInit(u16 Copy_u16Init);


/*
 * Public Function: 	Systick_vidSetTime
 *
 *
 * Description:			This function to set the time start of count
 *
 *
 * Input Parameter:		--> Copy_u32MilliSecond
 *
 *
 * Return:				Error Status
 *
 * */
Systick_tenuErrorStatus Systick_vidSetTime(u32 Copy_u32MilliSecond);



/*
 * Public Function: 	Systick_vidStart
 *
 *
 * Description:			This function to start countdown for systick
 *
 *
 * Input Parameter:		Pointer to function
 * 						--> Copy_pfCallBackFunction
 *
 * Return:				Void
 *
 * */
void Systick_vidStart(void);



/*
 * Public Function: 	Systick_vidRegisterCallBack
 *
 *
 * Description:			This function can be called by the user to assign a function to the
 * 						Systick interrupt
 *
 *
 * Input Parameter:		Pointer to function
 * 						--> Copy_pfCallBackFunction
 *
 * Return:				Void
 *
 * */

void Systick_vidRegisterCallBack(systickcfg_t Copy_pfCallBackFunction);


#endif /* SYSTICK_H_ */
