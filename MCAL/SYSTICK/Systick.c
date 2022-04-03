/*
 * Systick.c
 *
 *  Created on: Mar 28, 2022
 *      Author: fady
 */

#include "../../../Services/Std_types.h"
#include "Systick.h"



/*##################################################################*/
/*******************************types*******************************/

/*System register*/
typedef struct
{
	u32 Systck_Ctrl;
	u32 Systck_Load;
	u32 Systck_Value;
	u32 Systck_Calib;

}Systick_tstrRegister;


/*##################################################################*/


/*##################################################################*/
/***************************Defines*********************************/

#define SYSTCK	((Systick_tstrRegister*)0xE000E010)

#define MAX_VALUE	0x00FFFFFF
#define START		1
#define CHECK_MASK	0x1000


/*##################################################################*/
/**************************Global Variable**************************/

/*Variable to store function callback*/
static systickcfg_t Systick_prvAppNotify;

/*##################################################################*/


/*##################################################################*/
/***********************Fucntion Implementation*********************/

Systick_tenuErrorStatus Systick_vidInit(u16 Copy_u16Init)
{
	Systick_tenuErrorStatus Loc_enuErrorStatus = Systick_enuOK;

	if (Copy_u16Init & CHECK_MASK)
	{
		/*Local variable to collect the mask*/
		u8 Loc_u8CopyInit;

		/*Casting to remove the magic number*/
		Loc_u8CopyInit = (u8)Copy_u16Init;

		/*Clear all register values*/
		SYSTCK->Systck_Ctrl &= 0;

		/*write the mask to the register*/
		SYSTCK->Systck_Ctrl = Loc_u8CopyInit;

	}/*end of if --> checking Value*/

	else
	{
		/*Assign wrong value*/
		Loc_enuErrorStatus = Systick_enuWrongValue;

	}/*end of else --> error*/

	/*return status*/
	return Loc_enuErrorStatus;

}/*end of Systick_vidInit*/



Systick_tenuErrorStatus Systick_vidSetTime(u32 Copy_u32MilliSecond)
{

	Systick_tenuErrorStatus Loc_enuErrorStatus = Systick_enuOK;

	Copy_u32MilliSecond = CLOCK_FREQUENCY*Copy_u32MilliSecond;
	Copy_u32MilliSecond /= 1000;
	Copy_u32MilliSecond -= 1;
	/*check the value entered*/
	if (Copy_u32MilliSecond > MAX_VALUE)
	{
		Loc_enuErrorStatus = Systick_enuWrongValue;
	}/*end of if --> checking value*/
	/*delete value in the register*/

	else {

		SYSTCK->Systck_Load &= 0;

		/*Store the value in the register*/
		SYSTCK->Systck_Load = Copy_u32MilliSecond;
	}

	/*return status*/
	return Loc_enuErrorStatus;

}/*end of function Systick_vidSetTime*/



void Systick_vidStart(void)
{

	/*Set pin enable to 1*/
	SYSTCK->Systck_Ctrl |= START;

	/*return from function*/
	return;

}/*end of systick_vidStart function*/


void Systick_vidRegisterCallBack(systickcfg_t Copy_pfCallBackFunction)
{
	/*Check if the function not be null*/
	if (Copy_pfCallBackFunction)
	{
		/*Store the function address to the static variable*/
		Systick_prvAppNotify = Copy_pfCallBackFunction;

	}/*end of if --> checking function*/

	else
	{
		/*do nothing*/

	}/*end of else --> nothing*/

	/*return from function*/
	return;
}/*end of systick_vidRegisterCallback function*/


/*this function will be called by the hardware after the time countdown end*/
void SysTick_Handler (void)
{
	/*if static receive address of function enter the condition*/
	if (Systick_prvAppNotify)
	{
		/*Call the function in the application*/
		Systick_prvAppNotify();

	}/*end of if condition*/

	else
	{
		/*Do nothing*/

	}/*end of else*/

}/*end of Systick_handler*/

/*##################################################################*/
