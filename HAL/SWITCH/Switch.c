/*
 * Switch.c
 *
 *  Created on: Apr 4, 2022
 *      Author: fady
 */


#include "Std_types.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Switch_cfg.h"
#include "Switch.h"


extern Switch_tstrConfiguration Switch_astrSwitchs[];

static u8 ButtonStatus[NUMBER_OF_SWITCH];



Switch_tenuErrorStatus Switch_vidInit(void)
{
	u8 Loc_u8CheckingClockON = 0;
	u8 Loc_u8Counter = 0;

	Switch_tenuErrorStatus Loc_enuErrorStatus = Switch_enuOk;
	/*Create object from gpio configuration*/
	GPIO_tstrPinConfiguration Gpio_Configuration;


	/*For loop to loop around the pin to init*/
	for(Loc_u8Counter = 0; Loc_u8Counter < NUMBER_OF_SWITCH; Loc_u8Counter++)
	{

		Gpio_Configuration.GPIO_Mode = Switch_astrSwitchs[Loc_u8Counter].Mode;
		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Switch_astrSwitchs[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Switch_astrSwitchs[Loc_u8Counter].Port;
		/*Init the Pin speed*/

		if(Gpio_Configuration.GPIO_Port == PORTA && !(Loc_u8CheckingClockON & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOA);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x01;
		}

		else if(Gpio_Configuration.GPIO_Port == PORTB && !((Loc_u8CheckingClockON>>1) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOB);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x02;
		}

		else if(Gpio_Configuration.GPIO_Port == PORTC && !((Loc_u8CheckingClockON>>2) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOC);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x04;
		}

		else if(Gpio_Configuration.GPIO_Port == PORTD && !((Loc_u8CheckingClockON>>3) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOD);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x08;
		}

		else if(Gpio_Configuration.GPIO_Port == PORTE && !((Loc_u8CheckingClockON>>4) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOE);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x10;
		}

		else if(Gpio_Configuration.GPIO_Port == PORTH && !((Loc_u8CheckingClockON>>5) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOH);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x20;
		}

		/*Init the gpio for each led*/
		Loc_enuErrorStatus = Gpio_enuPinConfigurationInit(&Gpio_Configuration);

	}/*end for loop --> to init the led pin*/


	/*return status*/
	return Loc_enuErrorStatus;
}


Switch_tenuErrorStatus Switch_enuReadStatus(u8 Copy_u8SwitchNumber, pu8 Add_pu8PinStatus)
{
	Switch_tenuErrorStatus Loc_enuErrorStatus = Switch_enuOk;
	if (Copy_u8SwitchNumber > 16)
	{
		Loc_enuErrorStatus = Switch_enuWrongPin;
	}

	else if (Add_pu8PinStatus == NULL)
	{
		Loc_enuErrorStatus = Switch_enuNullPointer;
	}

	else
	{
		*Add_pu8PinStatus = ButtonStatus[Copy_u8SwitchNumber];
	}

	return Loc_enuErrorStatus;
}


void Switch_vidTask(void)
{
	static u32 Loc_u32prvCounter[NUMBER_OF_SWITCH];
	static u8 Loc_u32prvPrevValue[NUMBER_OF_SWITCH];
	u8 Loc_u8CurrentValue;
	u8 Loc_u8Iterator;

	for(Loc_u8Iterator = 0; Loc_u8Iterator < NUMBER_OF_SWITCH; Loc_u8Iterator++)
	{

		Gpio_enuGetPinValue(Switch_astrSwitchs[Loc_u8Iterator].Port, Loc_u8Iterator, &Loc_u8CurrentValue);

		if(Loc_u8CurrentValue == Loc_u32prvPrevValue[Loc_u8Iterator])
		{
			Loc_u32prvCounter[Loc_u8Iterator]++;
		}
		else
		{
			Loc_u32prvCounter[Loc_u8Iterator] = 0;
		}

		if (Loc_u32prvCounter[Loc_u8Iterator] == 5)
		{
			ButtonStatus[Loc_u8Iterator] = Loc_u8CurrentValue;
			Loc_u32prvCounter[Loc_u8Iterator] = 0;
		}

		Loc_u32prvPrevValue[Loc_u8Iterator] = Loc_u8CurrentValue;
	}
}
