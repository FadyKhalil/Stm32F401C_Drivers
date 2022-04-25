/*
 * Lcd.c
 *
 *  Created on: Apr 6, 2022
 *      Author: fady
 */

#include "Bit_utils.h"
#include "Std_types.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Lcd_cfg.h"
#include "Lcd.h"
#include "Lcd_prv.h"



typedef enum
{
	Lcd_enuStart = 0,
	Lcd_enuFunctionSet,
	Lcd_enuReturnHome,
	Lcd_enuClear,
	Lcd_enuDisplayControl,
	Lcd_enuEntryMode,
	Lcd_enuCursorDisplayControl,
	Lcd_enuHomePosition,
	Lcd_enuDone

}Lcd_tenuInitState;


typedef enum
{
	Lcd_enuNoRequest = 0,
	Lcd_enuWritingProcess,
	Lcd_enuSendCommand

}Lcd_tenuRequest;


typedef enum
{
	Lcd_enuGotoXY = 0,
	Lcd_enuWrite


}Lcd_tenuWriteProcess;

/*********************Private Variable***********************************/

static Lcd_tenuInitState Lcd_prvInitState = Lcd_enuStart;

static Lcd_tenuRequest Lcd_prvRequest = Lcd_enuNoRequest;

static u8* Lcd_prvString;

static u8 Lcd_prvLineNumber;

static u8 Lcd_prvColoneNumber;

static u8 Lcd_prvCommand;

/*******************************Extern Variable****************************/

extern Lcd_tstrPinConfiguration Lcd_strPin[];

/**************************Prototype Function Helper********************************/

void Lcd_vidHelperDisplayCharacter(u8 Copy_u8Character);

void Lcd_vidHelperGoto(u8 copy_u8Line, u8 copy_u8Colon);

void Lcd_vidHelperPinInit(void);

/**************************Prototype Function For Lcd_vidTask********************************/

static void Lcd_vidWriteProcess(void);

static void Lcd_vidCommand(u8 Copy_u8Command);

void Lcd_vidInitProcess(void);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void Lcd_vidInitProcess(void) {

	u8 loc_u8EnableValue = 0;

	switch(Lcd_prvInitState)
	{
	case Lcd_enuStart:

		Lcd_vidHelperPinInit();
		Lcd_prvInitState++;

		break;

	case Lcd_enuFunctionSet:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(LCD_FUNCSET);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	case Lcd_enuReturnHome:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(RETURN_HOME);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	case Lcd_enuClear:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(CLEAR);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	case Lcd_enuDisplayControl:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(LCD_DISPLAYCONTROL);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	case Lcd_enuEntryMode:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(LCD_ENTRY_MODE);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}
		break;

	case Lcd_enuCursorDisplayControl:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(LCD_CURSOR_DISPLAY_SH);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}
		break;

	case Lcd_enuHomePosition:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &loc_u8EnableValue);
		if (!loc_u8EnableValue)
		{
			Lcd_vidCommand(HOME_POSITION);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Lcd_prvInitState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	default:

		//Do nothing

		break;
	}

}


void Lcd_vidTask(void)
{
	if (Lcd_prvInitState != Lcd_enuDone)
	{
		Lcd_vidInitProcess();
	}
	else
	{
		switch(Lcd_prvRequest)
		{
		case Lcd_enuWritingProcess:

			Lcd_vidWriteProcess();

			break;

		case Lcd_enuSendCommand:

			Lcd_vidCommand(Lcd_prvCommand);

			break;

		case Lcd_enuNoRequest:

			// Do Nothing

			break;

		}
	}
}

/***************************Function Helper**********************************/


void Lcd_vidHelperDisplayCharacter(u8 Copy_u8Character) {


	LCD_tunByteAcces Loc_tunBitAccess;

	Loc_tunBitAccess.LCD_u8Comunication = Copy_u8Character;

	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8RS_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8RW_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);

	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D7_PIN].Port, Lcd_strPin[LCD_u8D7_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b7);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D6_PIN].Port, Lcd_strPin[LCD_u8D6_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b6);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D5_PIN].Port, Lcd_strPin[LCD_u8D5_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b5);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D4_PIN].Port, Lcd_strPin[LCD_u8D4_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b4);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D3_PIN].Port, Lcd_strPin[LCD_u8D3_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b3);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D2_PIN].Port, Lcd_strPin[LCD_u8D2_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b2);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D1_PIN].Port, Lcd_strPin[LCD_u8D1_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b1);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D0_PIN].Port, Lcd_strPin[LCD_u8D0_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b0);


}


//void Lcd_vidCreateCustomCharacter(u8 Copy_u8CgramAddress, pu8 Add_pu8CustomCharacter)
//{
//
//	u8 Loc_u8Iterator;
//	Lcd_vidSendCommand(0x40 + (Copy_u8CgramAddress*8));
//
//	for (Loc_u8Iterator = 0; Loc_u8Iterator < 8 ; Loc_u8Iterator++)
//	{
//		Lcd_vidDisplayCharacter(Add_pu8CustomCharacter[Loc_u8Iterator]);
//	}
//
//}


void Lcd_vidHelperGoto(u8 copy_u8Line, u8 copy_u8Colon)
{
	if (copy_u8Line == 0 && copy_u8Colon < 16)
	{
		Lcd_vidSendCommand((copy_u8Colon & 0x0F)|HOME_POSITION);
	}
	else if (copy_u8Line == 1 && copy_u8Colon < 16)
	{
		Lcd_vidSendCommand((copy_u8Colon & 0x0F)|LINE2);
	}
}


void Lcd_vidHelperPinInit(void)
{
	u8 Loc_u8CheckingClockON = 0;

	u8 Loc_u8Counter = 0;
	/*Create object from gpio configuration*/
	GPIO_tstrPinConfiguration Gpio_Configuration;

	/*For loop to loop around the pin to init*/
	for(Loc_u8Counter = 0; Loc_u8Counter < LCD_NUMBER_PIN ; Loc_u8Counter++)
	{

		Gpio_Configuration.GPIO_Mode = GPIO_u8OUTPUT_PUSHPULL;

		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Lcd_strPin[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Lcd_strPin[Loc_u8Counter].Port;
		/*Init the Pin speed*/
		Gpio_Configuration.GPIO_Speed = Lcd_strPin[Loc_u8Counter].Speed;


		// check if clock open or not
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
		Gpio_enuPinConfigurationInit(&Gpio_Configuration);

	}/*end for loop --> to init the led pin*/

	/*return status*/
	return;

}
/******************************Function Used by Lcd_vidTask*****************************/

static void Lcd_vidWriteProcess(void)
{
	static Lcd_tenuWriteProcess Loc_prvState = Lcd_enuGotoXY;

	static u8 loc_u8Iterator = 0;

	u8 Loc_u8Enable = 0;

	switch(Loc_prvState)
	{
	case Lcd_enuGotoXY:
		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &Loc_u8Enable);
		if (!Loc_u8Enable)
		{
			Lcd_vidHelperGoto(Lcd_prvLineNumber, Lcd_prvColoneNumber);
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
		}
		else
		{
			Loc_prvState++;
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
		}

		break;

	case Lcd_enuWrite:

		Gpio_enuGetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, &Loc_u8Enable);

		if (Lcd_prvString[loc_u8Iterator] != '\0')
		{
			Lcd_vidHelperDisplayCharacter(Lcd_prvString[loc_u8Iterator]);
			loc_u8Iterator++;
		}
		else if (!Loc_u8Enable)
		{
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8HIGH);
			Loc_prvState++;
		}
		else
		{
			Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
			Lcd_prvRequest = Lcd_enuNoRequest;
			Loc_prvState = Lcd_enuGotoXY;
		}

		break;

	}
}


static void Lcd_vidCommand(u8 Copy_u8Command)
{

	LCD_tunByteAcces Loc_tunBitAccess;

	Loc_tunBitAccess.LCD_u8Comunication = Copy_u8Command;

	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8RS_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8RS_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8EN_PIN].Port, Lcd_strPin[LCD_u8EN_PIN].Pin, GPIO_u8LOW);

	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D7_PIN].Port, Lcd_strPin[LCD_u8D7_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b7);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D6_PIN].Port, Lcd_strPin[LCD_u8D6_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b6);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D5_PIN].Port, Lcd_strPin[LCD_u8D5_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b5);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D4_PIN].Port, Lcd_strPin[LCD_u8D4_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b4);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D3_PIN].Port, Lcd_strPin[LCD_u8D3_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b3);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D2_PIN].Port, Lcd_strPin[LCD_u8D2_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b2);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D1_PIN].Port, Lcd_strPin[LCD_u8D1_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b1);
	Gpio_enuSetPinValue(Lcd_strPin[LCD_u8D0_PIN].Port, Lcd_strPin[LCD_u8D0_PIN].Pin, Loc_tunBitAccess.LCD_strBitCommunication.b0);

}

/******************************Function Called By user*****************************/

Lcd_tenuState Lcd_vidDisplayStringPosition(Lcd_tstrDisplayChrPosition* Copy_strChrLocation)
{

	Lcd_tenuState Loc_enuState = Lcd_enuProccessing;

	if (Lcd_prvRequest == Lcd_enuNoRequest)
	{
	Lcd_prvString = Copy_strChrLocation->string;
	Lcd_prvLineNumber = Copy_strChrLocation->LineNumber;
	Lcd_prvColoneNumber = Copy_strChrLocation->ColoneNumber;

	Lcd_prvRequest = Lcd_enuWritingProcess;

	}

	else
	{
		Loc_enuState = Lcd_enuBusy;
	}

	return Loc_enuState;

}


Lcd_tenuState Lcd_vidSendCommand(u8 Copy_u8Command)
{
	Lcd_tenuState Loc_enuState = Lcd_enuProccessing;

	if (Lcd_prvRequest == Lcd_enuNoRequest)
	{
		Lcd_prvCommand = Copy_u8Command;
		Lcd_prvRequest = Lcd_enuSendCommand;
	}

	else
	{
		Loc_enuState = Lcd_enuBusy;
	}

	return Loc_enuState;
}

