/*
 * Keypad.c
 *
 *  Created on: Apr 10, 2022
 *      Author: fady
 */

#include "../../Services/Std_types.h"
#include "../../MCAL/GPIO/Gpio.h"
#include "../../MCAL/RCC/Rcc.h"
#include "Keypad.h"
#include "Keypad_cfg.h"

extern Keypad_tsConfig Keypad_Config_Ouput[];
extern Keypad_tsConfig Keypad_Config_Input[];

static u8 Keypad_u8PressedKey = 0;
static u8 Keypad_u8NewValueFlag = 0;

static u8 Keypad_u8arrValues[] = "123/456*789+.0.-";

Keypad_tenuErrorStatus KeyPad_vidInit(void)
{
	u8 Loc_u8CheckingClockON = 0;
	u8 Loc_u8Counter = 0;

	Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuOk;
	/*Create object from gpio configuration*/
	GPIO_tstrPinConfiguration Gpio_Configuration;

	/*For loop to loop around the pin to init*/
	for (Loc_u8Counter = 0; Loc_u8Counter < KEYPAD_u8ROW_COUNT; Loc_u8Counter++)
	{

		Gpio_Configuration.GPIO_Mode = Keypad_Config_Ouput[Loc_u8Counter].Mode;
		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Keypad_Config_Ouput[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Keypad_Config_Ouput[Loc_u8Counter].PORT;
		/*Init the Pin speed*/

		if (Gpio_Configuration.GPIO_Port == PORTA && !(Loc_u8CheckingClockON & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOA);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x01;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTB && !((Loc_u8CheckingClockON >> 1) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOB);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x02;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTC && !((Loc_u8CheckingClockON >> 2) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOC);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x04;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTD && !((Loc_u8CheckingClockON >> 3) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOD);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x08;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTE && !((Loc_u8CheckingClockON >> 4) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOE);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x10;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTH && !((Loc_u8CheckingClockON >> 5) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOH);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x20;
		}

		/*Init the gpio for each led*/
		Loc_enuErrorStatus = Gpio_enuPinConfigurationInit(&Gpio_Configuration);

	}/*end for loop --> to init the led pin*/

	/*For loop to loop around the pin to init*/
	for (Loc_u8Counter = 0; Loc_u8Counter < KEYPAD_u8COLUMN_COUNT; Loc_u8Counter++)
	{

		Gpio_Configuration.GPIO_Mode = Keypad_Config_Input[Loc_u8Counter].Mode;
		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Keypad_Config_Input[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Keypad_Config_Input[Loc_u8Counter].PORT;
		/*Init the Pin speed*/

		if (Gpio_Configuration.GPIO_Port == PORTA && !(Loc_u8CheckingClockON & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOA);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x01;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTB && !((Loc_u8CheckingClockON >> 1) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOB);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x02;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTC && !((Loc_u8CheckingClockON >> 2) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOC);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x04;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTD && !((Loc_u8CheckingClockON >> 3) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOD);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x08;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTE && !((Loc_u8CheckingClockON >> 4) & 0x01))
		{
			Rcc_SetAHB1Peripheral(SET_RESET_GPIOE);
			Loc_u8CheckingClockON = Loc_u8CheckingClockON | 0x10;
		}

		else if (Gpio_Configuration.GPIO_Port == PORTH && !((Loc_u8CheckingClockON >> 5) & 0x01))
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

void Keypad_GetKeyValue(void)
{
	static Switch_Status_e SwitchState[KEYPAD_u8ROW_COUNT * KEYPAD_u8COLUMN_COUNT] =
	{ Init };
	u8 Loc_u8Row = 0;
	u8 Loc_u8Column = 0;
	u8 Loc_u8CurrentValue = 0;
	u8 Loc_u8CurrentPressedKey = 0;
	while ((Loc_u8Row < KEYPAD_u8ROW_COUNT) && (Loc_u8CurrentPressedKey == 0))
	{
		Gpio_enuSetPinValue(Keypad_Config_Ouput[Loc_u8Row].PORT, Keypad_Config_Ouput[Loc_u8Row].Pin, GPIO_u8HIGH);
		Loc_u8Column = 0;
		while ((Loc_u8Column < KEYPAD_u8COLUMN_COUNT) && (Loc_u8CurrentPressedKey == 0))
		{
			switch (SwitchState[(Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column])
			{
			case Init:
				Gpio_enuGetPinValue(Keypad_Config_Input[Loc_u8Column].PORT, Keypad_Config_Input[Loc_u8Column].Pin, &Loc_u8CurrentValue);
				if (Loc_u8CurrentValue == GPIO_u8HIGH)
				{
					SwitchState[(Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column] = Pressed;
				}
				else
				{
					SwitchState[(Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column] = Not_Pressed;
				}
				break;

			case Pressed:
				Gpio_enuGetPinValue(Keypad_Config_Input[Loc_u8Column].PORT, Keypad_Config_Input[Loc_u8Column].Pin, &Loc_u8CurrentValue);
				if (Loc_u8CurrentValue == GPIO_u8HIGH)
				{
					Loc_u8CurrentPressedKey = (Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column;
				}
				else
				{
					SwitchState[(Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column] = Not_Pressed;
				}
				break;

			case Not_Pressed:
				Gpio_enuGetPinValue(Keypad_Config_Input[Loc_u8Column].PORT, Keypad_Config_Input[Loc_u8Column].Pin, &Loc_u8CurrentValue);
				if (Loc_u8CurrentValue == GPIO_u8HIGH)
				{
					SwitchState[(Loc_u8Row * KEYPAD_u8ROW_COUNT) + Loc_u8Column] = Pressed;
				}
				break;
			}
			Loc_u8Column++;
		}
		Gpio_enuSetPinValue(Keypad_Config_Ouput[Loc_u8Row].PORT, Keypad_Config_Ouput[Loc_u8Row].Pin, GPIO_u8LOW);
		Loc_u8Row++;
	}
	if (Loc_u8CurrentPressedKey != Keypad_u8PressedKey)
	{
		Keypad_u8PressedKey = Loc_u8CurrentPressedKey;
		Keypad_u8NewValueFlag = 1;
	}
}

extern void Keypad_u8GetPressedKey(pu8 Add_pu8Key)
{
	if(Add_pu8Key)
	{
		if ((Keypad_u8NewValueFlag == 1) && (Keypad_u8PressedKey != 0))
		{
			*Add_pu8Key = Keypad_u8arrValues[Keypad_u8PressedKey];
			Keypad_u8NewValueFlag = 0;
		}
		else
		{
			*Add_pu8Key = 0;
		}
	}
	else
	{
		//do nothing
	}
}

