/*
 * Rcc.c
 *
 *  Created on: Mar 12, 2022
 *      Author: fady
 */

#include "../../../../Services/Std_types.h"
#include "Rcc.h"

/*-------------------------------------------------------------------------------*/
typedef struct
{
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved[2];
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved1[2];
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved2[2];
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
	volatile u32 AHB1LPENR;
	volatile u32 AHB2LPENR;
	volatile u32 Reserved3[2];
	volatile u32 APB1LPENR;
	volatile u32 APB2LPENR;
	volatile u32 Reserved4[2];
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 Reserved5[2];
	volatile u32 SSCGR;
	volatile u32 PLLI2SCFGR;
	volatile u32 RESERVED6;
	volatile u32 DCKCFGR;


}Rcc_tstrRegisterType;


/*-------------------------------------DEFINES--------------------------------------*/

/*RCC Based Address*/
#define RCC_u32BASEADDRESS  0x40023800

/*Make the structure point at the base address of RCC*/
#define RCC		((Rcc_tstrRegisterType* )(RCC_u32BASEADDRESS))

#define CLEAR_MAGIC_CFGR	0xCFFFFFFF

#define CLEAR_MAGIC_AHB1RSRR	0xBFFFFFFF

#define CLEAR_MAGIC_APB2RSRR	0xAFFFFFFF

#define CLEAR_MAGIC_APB1RSRR	0x9FFFFFFF


/*----------------------------------------------------------------------------------------*/



/*
 * Public Function  		:	Rcc_SetClockOn
 * Description				:	This Function is to set the clock chosen ON
 *
 * Input Parameter:
 * @	Copy_u8ClockSelection:	in range {HSI_CLOCK, HSEON_CLOCK, PLLON_CLOCK}
 * !		The input represent the Mask of the clock
 * !		Protected with magic number
 *
 * Return					:	Return Error Status if any from the input
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus Rcc_SetClockOn(u32 Copy_u32ClockSelection)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus;

	Loc_tenuErrorStatus = Rcc_OK;

	switch(Copy_u32ClockSelection)
	{
	case HSI_CLOCK:

		/*Clear the magic bit and select bit 0 from register CR to Set HSI*/
		RCC->CR |= 0x00000001;
		break;

	case HSE_CLOCK:

		/*Clear the magic bit and select bit 16 from register CR to set HSE*/
		RCC->CR |= 0x00010000;
		break;

	case PLL_CLOCK:

		/*Clear the magic bit and select bit 24 from register CR to set HSE*/
		RCC->CR |= 0x01000000;
		break;

	default:
		Loc_tenuErrorStatus = Rcc_InputError;
		break;
	}

	return Loc_tenuErrorStatus;
}


/*
 * Public Function  		:	Rcc_ReadStatusClock
 * Description				:	This Function is to ready the status of the clock
 *
 * Input Parameter:
 * @	Copy_u8ClockSelection:	in range {HSI_CLOCK, HSEON_CLOCK, PLLON_CLOCK}
 * !		The input represent the Mask of the clock
 * !		Protected with magic number
 *
 * Return					:	-> Return Error Status if any from the input
 * 								-> if the clock didn't get ready
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_ReadStatusClock(u32 Copy_u8ClockSelection)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus;

	Loc_tenuErrorStatus = Rcc_OK;
	u32	timer = 50;


	/*Copy the register to value to this variable*/
	switch(Copy_u8ClockSelection)
	{
	case HSI_CLOCK:

		/*Set bit of the representation of HSIRDY bit in the register*/
		Copy_u8ClockSelection = 0x00000002;

		/*Check the two condition whether the HSIRDY will raise or the timer end*/
		while(timer && !(RCC->CR & Copy_u8ClockSelection))
		{
			timer--;
		}
		if (!(RCC->CR & Copy_u8ClockSelection))
		{
			Loc_tenuErrorStatus = Rcc_ClockNotReady;
		}
		else
		{
			//  Do Nothing
		}
		break;


	case HSE_CLOCK:

		/*Set bit of the representation of HSERDY bit in the register*/
		Copy_u8ClockSelection= 0x00020000;

		/*Check the two condition whether the HSERDY will raise or the timer end*/
		while(timer && !(RCC->CR & Copy_u8ClockSelection))
		{
			timer--;
		}
		if (!(RCC->CR & Copy_u8ClockSelection))
		{
			Loc_tenuErrorStatus = Rcc_ClockNotReady;
		}
		else
		{
			//  Do Nothing
		}
		break;

	case PLL_CLOCK:

		/*Set bit of the representation of PLLRDY bit in the register*/
		Copy_u8ClockSelection= 0x02000000;

		/*Check the two condition whether the PLLRDY will raise or the timer end*/
		while(timer && !(RCC->CR & Copy_u8ClockSelection))
		{
			timer--;
		}
		if (!(RCC->CR & Copy_u8ClockSelection))
		{
			Loc_tenuErrorStatus = Rcc_ClockNotReady;
		}
		else
		{
			//  Do Nothing
		}
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}

	return Loc_tenuErrorStatus;
}


/*
 * Public Function  		:	Rcc_SetHseClockBypass
 * Description				:	This Function is to set the bypass clock to bypass the oscillator with an external clock
 *
 *!!!Constrain				:	You can only set it if and only if the you didn't set the clock system
 *
 * Input Parameter:
 * @	Not-Applicable	(void)
 *
 * Return					:	Not-Applicable (void)
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
void Rcc_SetHseClockBypass(void)
{
	RCC->CR |= 1<<16;
}


Rcc_tenuErrorStatus Rcc_SetPllClock(u32 Copy_u32ClockSelection)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;
	u32 Loc_u32ReadRccCfgrRgst;

	/*Store the register PLL configuration in variable to delete the magic number*/
	Loc_u32ReadRccCfgrRgst = RCC->PLLCFGR;

	/*To check the PLL is on*/
	if(RCC->CR & (PLL_CLOCK & 0xDFFFFFFF))
	{
		Loc_tenuErrorStatus = Rcc_PllEnable;
	}

	else
	{
		switch (Copy_u32ClockSelection)
		{
		case PLL_HSI:

			/*To set clock HSI to be the clock usage for PLL we need to clear the bit 22*/
			Loc_u32ReadRccCfgrRgst &= 0xFFBFFFFF;
			RCC->PLLCFGR = Loc_u32ReadRccCfgrRgst;
			break;

		case PLL_HSE:

			/*To set clock HSE to be the clock usage for PLL we need to set the bit 22*/
			Loc_u32ReadRccCfgrRgst |= 0x00400000;
			RCC->PLLCFGR = Loc_u32ReadRccCfgrRgst;
			break;

		default:

			/*Checking for wrong input*/
			Loc_tenuErrorStatus = Rcc_InputError;
			break;

		}
	}
	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_PllConfiguration(u8 Copy_u8PllM, u16 Copy_u16PllN, u8 Copy_u8PllQ, u8 Copy_u8PllP)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;
	u32 Loc_u32ClearVariable;
	/*To check the PLL is on*/
	if(RCC->CR & (PLL_CLOCK & 0xDFFFFFFF))
	{
		Loc_tenuErrorStatus = Rcc_PllEnable;
	}

	/*To check the input PllM*/
	else if (Copy_u8PllM < 2 || Copy_u8PllM > 63)
	{
		Loc_tenuErrorStatus = Rcc_InputRangeError;
	}

	/*To check the input PllN*/
	else if (Copy_u16PllN < 192 || Copy_u16PllN > 432)
	{
		Loc_tenuErrorStatus = Rcc_InputRangeError;
	}

	/*To check the input PllQ*/
	else if (Copy_u8PllQ < 2 || Copy_u8PllQ > 15)
	{
		Loc_tenuErrorStatus = Rcc_InputRangeError;
	}

	else
	{

		/*To clear all values in the register PLL Configuration*/
		Loc_u32ClearVariable = 0x00400000;
		RCC->PLLCFGR &= Loc_u32ClearVariable;

		/*Set the values of the configuration*/
		Loc_u32ClearVariable = Copy_u8PllQ<<24 | Copy_u8PllP<<16 | Copy_u16PllN<<6 | Copy_u8PllM;
		RCC->PLLCFGR |= Loc_u32ClearVariable;

	}

	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_SetClockSystem (u32 Copy_u32ClockSlection)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;


	/*Switch case to select the clock needed to be system clock*/
	switch (Copy_u32ClockSlection)
	{
	case HSI_CLOCK_SYSTEM:

		/*Set HSI as system clock and clear the magic number*/
		RCC->CFGR &= (HSI_CLOCK_SYSTEM & CLEAR_MAGIC_CFGR);

		break;

	case HSE_CLOCK_SYSTEM:

		/*Set HSE as system clock and clear the magic number*/
		RCC->CFGR = (HSE_CLOCK_SYSTEM & CLEAR_MAGIC_CFGR);

		break;

	case PLL_CLOCK_SYSTEM:

		/*Set PLL as system clock and clear the magic number*/
		RCC->CFGR = (PLL_CLOCK_SYSTEM & CLEAR_MAGIC_CFGR);

		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}


	return Loc_tenuErrorStatus;

}


Rcc_tenuClockSystem Rcc_ReadSystemClockStatus (void)
{
	Rcc_tenuClockSystem Loc_tenuClockSytem;
	u32 Loc_u32ClockStatus;


	Loc_u32ClockStatus = RCC->CFGR;

	Loc_u32ClockStatus &= 0x0000000C;

	if (Loc_u32ClockStatus & HSE_CLOCK_SYSTEM_STATUS)
	{
		Loc_tenuClockSytem = Rcc_HseClockSytem;
	}

	else if (Loc_u32ClockStatus & PLL_CLOCK_SYSTEM_STATUS)
	{
		Loc_tenuClockSytem = Rcc_PllClockSytem;
	}

	else
	{
		Loc_tenuClockSytem = Rcc_HsiClockSytem;
	}

	return Loc_tenuClockSytem;
}


Rcc_tenuErrorStatus	Rcc_ResetAHB1Peripheral(u32 Copy_u32AHB1Peripheral)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;


	switch(Copy_u32AHB1Peripheral)
	{
	case SET_RESET_GPIOA:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOB:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOC:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOD:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOE:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOH:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1RSTR;
		RCC->AHB1RSTR = Copy_u32AHB1Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}

	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_ResetAPB2Peripheral(u32 Copy_u32APB2Peripheral)
{

	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;

	switch(Copy_u32APB2Peripheral)
	{

	case SET_RESET_TIM1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_USART1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_USART6:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_ADC1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SDIO:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SPI1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SPI4:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SYSTEMCFGR:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM9:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM10:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM11:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2RSTR;
		RCC->APB2RSTR = Copy_u32APB2Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;

	}


	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_ResetAPB1Peripheral(u32 Copy_u32APB1Peripheral)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;

	switch(Copy_u32APB1Peripheral)
	{

	case SET_RESET_TIM2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM4:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM5:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_WWDG:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_SPI2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_SPI3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_USART2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C1:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1RSTR;
		RCC->APB1RSTR = Copy_u32APB1Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}



	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_SetAHB1Peripheral(u32 Copy_u32AHB1Peripheral)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;


	switch(Copy_u32AHB1Peripheral)
	{
	case SET_RESET_GPIOA:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1LPENR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOB:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1ENR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOC:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1ENR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOD:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1ENR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOE:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1ENR = Copy_u32AHB1Peripheral;
		break;

	case SET_RESET_GPIOH:
		Copy_u32AHB1Peripheral &= CLEAR_MAGIC_AHB1RSRR;
		Copy_u32AHB1Peripheral |= RCC->AHB1ENR;
		RCC->AHB1ENR = Copy_u32AHB1Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}

	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_SetAPB2Peripheral(u32 Copy_u32APB2Peripheral)
{

	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;

	switch(Copy_u32APB2Peripheral)
	{

	case SET_RESET_TIM1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_USART1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_USART6:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_ADC1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SDIO:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SPI1:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SPI4:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_SYSTEMCFGR:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM9:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM10:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	case SET_RESET_TIM11:
		Copy_u32APB2Peripheral &= CLEAR_MAGIC_APB2RSRR;
		Copy_u32APB2Peripheral |= RCC->APB2ENR;
		RCC->APB2ENR = Copy_u32APB2Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;

	}


	return Loc_tenuErrorStatus;
}


Rcc_tenuErrorStatus	Rcc_SetAPB1Peripheral(u32 Copy_u32APB1Peripheral)
{
	Rcc_tenuErrorStatus Loc_tenuErrorStatus = Rcc_OK;

	switch(Copy_u32APB1Peripheral)
	{

	case SET_RESET_TIM2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM4:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_TIM5:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_WWDG:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_SPI2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_SPI3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_USART2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C1:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C2:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	case SET_RESET_I2C3:
		Copy_u32APB1Peripheral &= CLEAR_MAGIC_APB1RSRR;
		Copy_u32APB1Peripheral |= RCC->APB1ENR;
		RCC->APB1ENR = Copy_u32APB1Peripheral;
		break;

	default:

		Loc_tenuErrorStatus = Rcc_InputError;

		break;
	}


	return Loc_tenuErrorStatus;
}


