/*
 * Ext.c
 *
 *  Created on: Apr 19, 2022
 *      Author: fady
 */

#include "../../Services/Std_types.h"
#include "Ext.h"


static EXTI_Cbf	EXTI_prvCallBack[3];

typedef struct
{
	u32 IMR;
	u32 EMR;
	u32 RTSR;
	u32 FTSR;
	u32 SWIER;
	u32 PR;

}External_Register_t;

typedef struct
{
	u32 MEMRMP;
	u32 PMC;
	u32 EXT1CR1;
	u32 EXT1CR2;
	u32 EXT1CR3;
	u32 EXT1CR4;
	u32 CMPCR;

}Syscfg_Register_t;

/*0x08*/

#define SYSCFG	((Syscfg_Register_t*)0x40013800)


#define EXTI	((External_Register_t*)(0x40013C00))



void External_vidInit(External_Config_t* ConfigPtr)
{

	if (ConfigPtr)
	{

		if(ConfigPtr->Mode == INTERRUPT_MASKABLE)
		{
			EXTI->IMR |= (ConfigPtr->Interrupt_Condition) << ConfigPtr->Channel;
		}
		else if(ConfigPtr->Mode == EVENT_MASKABLE)
		{
			EXTI->EMR |= (ConfigPtr->Interrupt_Condition) << ConfigPtr->Channel;
		}

		EXTI->FTSR |= (ConfigPtr->TriggerFalling) << ConfigPtr->Channel;
		EXTI->RTSR |= (ConfigPtr->TriggerRising) << ConfigPtr->Channel;

		SYSCFG->EXT1CR1 = (ConfigPtr->Pin) << ((ConfigPtr->Channel) * 4);

	}
	else
	{
		//pointerError
	}
}

void External_vidEnable(Channel_t Channel)
{
	if(Channel < 16)
		EXTI->IMR |= 1 << Channel;
	else
	{
		//Error
	}
}

void External_vidDisable(Channel_t Channel)
{
	if(Channel < 16)
		EXTI->IMR &= ~(1 << Channel);
	else
	{
		//Error
	}
}

void External_vidSoftwareInt(External_Software_t* ConfigPtr)
{
	if(ConfigPtr)
	{
		if(EXTI->IMR >> ConfigPtr->Channel)
		{
			EXTI->SWIER = (ConfigPtr->Software_Int_State) << ConfigPtr->Channel;
		}
		else
		{
			//Do nothing
		}
	}
	else
	{
		//error
	}
}

void External_vidDisablePending(Channel_t Channel)
{
	if(Channel < 16)
		EXTI->PR |= 1 << Channel;

	else
	{
		//error
	}
}


void External_vidRegisterCallBack(EXTI_Cbf CallBack, Channel_t Channel)
{
	if(CallBack)
	{
		EXTI_prvCallBack[Channel] = CallBack;
	}
	else
	{
		//error
	}
}


void EXTI0_IRQHandler(void)
{
	if(EXTI_prvCallBack[EXT0])
	{
		EXTI_prvCallBack[EXT0]();
		EXTI->PR |= 1 << EXT0;
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_prvCallBack[EXT1])
	{
		EXTI_prvCallBack[EXT1]();
		EXTI->PR |= 1 << EXT1;
	}
}
void EXTI2_IRQHandler(void)
{
	if(EXTI_prvCallBack[EXT2])
	{
		EXTI_prvCallBack[EXT2]();
		EXTI->PR |= 1 << EXT2;
	}
}
