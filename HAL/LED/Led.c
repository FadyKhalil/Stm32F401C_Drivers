/*
 * Led.c
 *
 *  Created on: Mar 27, 2022
 *      Author: fady
 */

#include "Std_types.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Led_cfg.h"
#include "Led.h"


extern Led_tstrConfiguration Led_astrleds[];

Led_tenuErrorStatus Led_enuInit(void)
{

	u8 Loc_u8CheckingClockON = 0;
	Led_tenuErrorStatus Loc_enuErrorStatus = Led_enuOk;
	u8 Loc_u8Counter = 0;
	/*Create object from gpio configuration*/
	GPIO_tstrPinConfiguration Gpio_Configuration;

	/*For loop to loop around the pin to init*/
	for(Loc_u8Counter = 0; Loc_u8Counter < LED_NUMBER; Loc_u8Counter++)
	{
		if (Led_astrleds[Loc_u8Counter].Otype == LED_OTYPE_PP)
		{
			Gpio_Configuration.GPIO_Mode = GPIO_u8OUTPUT_PUSHPULL;
		}
		else
		{
			Gpio_Configuration.GPIO_Mode = GPIO_u8OUTPUT_OPENDRAIN;
		}

		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Led_astrleds[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Led_astrleds[Loc_u8Counter].Port;
		/*Init the Pin speed*/
		Gpio_Configuration.GPIO_Speed = Led_astrleds[Loc_u8Counter].Speed;

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

}/*end of Led_enuInit*/



Led_tenuErrorStatus Led_enuStatus(u16 Copy_u16LedNumber, u8 Copy_u8LedStatus)
{
	Led_tenuErrorStatus Loc_enuErrorStatus	= Led_enuOk;


	/*Set Pin value
	 *
	 * ---> Getting the Port Number by the user for the configuration .c file
	 *
	 * ---> Getting number of pin by the user array
	 *
	 * ---> Setting the status by using XOR in case of active state
	 *
	 * */
	Loc_enuErrorStatus = Gpio_enuSetPinValue(Led_astrleds[Copy_u16LedNumber].Port, Copy_u16LedNumber,
												Copy_u8LedStatus ^ Led_astrleds[Copy_u16LedNumber].Active_states);

	/*return status*/
	return Loc_enuErrorStatus;

}/*end of Led_enuStatus*/


