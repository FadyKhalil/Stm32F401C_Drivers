/*
 * Gpio.c
 *
 *  Created on: Mar 20, 2022
 *      Author: fady
 */


#include "../MCAL/Gpio.h"

#include "../Services/Std_types.h"


/**********************************************************/
/**************************Types**************************/

/*Typedef structure for Register Peripheral Access*/
typedef struct
{
	u32 Moder;
	u32 Otyper;
	u32 Ospeedr;
	u32 Pupdr;
	u32 Idr;
	u32 Odr;
	u32 Bsrr;
	u32 Lckr;
	u32 Afrl;
	u32 Afrh;

}Gpio_tstrRegister;



/**********************************************************/
/*************************Defines*************************/

/*NULL POINTER*/
#define	NULL	(void*)0


/*Defining the GPIO register base to be point to the register belong to it*/

#define GPIOA				((*(GPIO_Register *)0x40020000))
#define GPIOB				((*(GPIO_Register *)0x40020400))
#define GPIOC				((*(GPIO_Register *)0x40020800))
#define GPIOD				((*(GPIO_Register *)0x40020C00))
#define GPIOE				((*(GPIO_Register *)0x40021000))
#define GPIOH				((*(GPIO_Register *)0x40021C00))


/*Clear the bit of each configuration of MODER, OTYPER, PUPDR Register*/

#define	CLEAR_MODER_LSB			0xFFFFFFFC
#define CLEAR_OTYPER_LSB		0xFFFFFFFB
#define CLEAR_PUPDR_LSB			0xFFFFFFE7

/*Mode Cases*/
#define MODE_00		0
#define MODE_01		1
#define MODE_10		2
#define MODE_11		3


/*################################################################################*/
/*****************************Function Prototype Helper****************************/

/*Function Helper for input Initialization*/
Gpio_tenuErrorStatus Gpio_enuHelperInput(GPIO_tstrPinConfiguration* Add_pstrPinConfg);


/*Function Helper for Output Initialization*/
Gpio_tenuErrorStatus Gpio_enuHelperOutput(GPIO_tstrPinConfiguration* Add_pstrPinConfg);


/*Function Helper for Output Alternative Function Initialization*/
Gpio_tenuErrorStatus Gpio_enuHelperAfOutput(GPIO_tstrPinConfiguration* Add_pstrPinConfg);


/*Function Helper for Analog Initialization*/
Gpio_tenuErrorStatus Gpio_enuHelperAnalog(GPIO_tstrPinConfiguration* Add_pstrPinConfg);

/*####################################################################################*/



/*####################################################################################*/
/*********************************Function Implementation******************************/

Gpio_tenuErrorStatus Gpio_enuPinConfigurationInit(GPIO_tstrPinConfiguration* Add_pstrPinConfg)
{

	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;
	u8 Loc_u8Mode;

	/*Check Null Pointer*/
	if (Add_pstrPinConfg == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}/*End of If checking Null Pointer*/

	else
	{
		/*Read the value of Mode of the Pin*/
		Loc_u8Mode = (Add_pstrPinConfg->GPIO_Mode) >> 3;

		switch(Loc_u8Mode)
		{

		case MODE_00:
			Loc_enuErrorStatus = Gpio_enuHelperInput(Add_pstrPinConfg);
			break;

		case MODE_01:
			Loc_enuErrorStatus = Gpio_enuHelperOutput(Add_pstrPinConfg);
			break;

		case MODE_10:
			Loc_enuErrorStatus = Gpio_enuHelperAfOutput(Add_pstrPinConfg);
			break;

		case MODE_11:
			Loc_enuErrorStatus = Gpio_enuHelperAnalog(Add_pstrPinConfg);
			break;

		}/*End of Switch Case for Mode*/

	}/*End of Else*/

	/*Return Error Status*/
	return Loc_enuErrorStatus;

}/*End of Function Gpio_PinConfigurationInit*/


Gpio_tenuErrorStatus Gpio_enuSetPinValue(void* Add_vidGpioPort, u16 Copy_u8GpioPinNumber, u8 Copy_u8GpioPinValue)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;

	u32 Loc_u32BsrrVariable = 0;

	if (Copy_u8GpioPinValue > 2)
	{
		Loc_enuErrorStatus = Gpio_enuPinValueError;
	}

	else
	{

		if (Copy_u8GpioPinValue == GPIO_u8HIGH)
		{
			Loc_u32BsrrVariable = ((Gpio_tstrRegister*)Add_vidGpioPort)->Bsrr;
			Loc_u32BsrrVariable |= Copy_u8GpioPinNumber;

			((Gpio_tstrRegister*)Add_vidGpioPort)->Bsrr = Loc_u32BsrrVariable;

		}/*end of if For High status*/

		else if (Copy_u8GpioPinValue == GPIO_u8LOW)
		{

			Loc_u32BsrrVariable =((Gpio_tstrRegister*)Add_vidGpioPort)->Bsrr;
			Loc_u32BsrrVariable = Copy_u8GpioPinNumber << 16;

			((Gpio_tstrRegister*)Add_vidGpioPort)->Bsrr = Loc_u32BsrrVariable;

		}/*end of else if for low status*/

	}/*end of else*/

	/*return Error Status*/
	return Loc_enuErrorStatus;

}/*end of function Gpio_enuSetPinValue*/

Gpio_tenuErrorStatus Gpio_enuGetPinValue(void* Add_vidGpioPort, u16 Copy_u8GpioPinNumber, pu8 Add_pu8GpioPinValue)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;

	/*Checking pin entered by the user*/
	if (Copy_u8GpioPinNumber > 15)
	{
		/*Check the error status*/
		Loc_enuErrorStatus = Gpio_enuPinNumberError;

	}/*end of If*/

	else if (Add_pu8GpioPinValue == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}
	else
	{
		/*Get the pin Value*/
		*Add_pu8GpioPinValue = ((Gpio_tstrRegister*)Add_vidGpioPort)->Idr & Copy_u8GpioPinNumber;

	}/*end of else*/

	/*Return the status*/
	return Loc_enuErrorStatus;

}/*end of function Gpio_enuGetPinValue*/



/*###########################################################################################*/
/****************************Implementation of Helper Function********************************/

Gpio_tenuErrorStatus Gpio_enuHelperInput(GPIO_tstrPinConfiguration* Add_pstrPinConfg)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;
	u8 Loc_u8Counter;
	u8 Loc_u8InputPupdr = (Add_pstrPinConfg->GPIO_Mode & 0x03);
	u16 Loc_u8PinValue = Add_pstrPinConfg->GPIO_Pin;
	u32 Loc_u32TempRegister;

	/*Checking Null Pointer*/
	if (Add_pstrPinConfg == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}/*end of if */

	/*Checking port null pointer*/
	else if (Add_pstrPinConfg->GPIO_Port == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}/*end of else if*/

	else
	{
		for (Loc_u8Counter = 0; Loc_u8Counter < 16; Loc_u8Counter++)
		{
			/*CHeck if pin exit*/
			if ((1 << Loc_u8Counter) & Loc_u8PinValue)
			{
				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder;

				/*Init the Pin as input*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Store the new variable*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder = Loc_u32TempRegister;

				/*Store the register PUPDR temporary*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr;

				/*Clear the bit for the pin chosen*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Store the bit mode*/
				Loc_u32TempRegister |= (Loc_u8InputPupdr << (Loc_u8Counter << 1));

				/*Store the temporary variable to the register*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr = Loc_u32TempRegister;

			}/*end of if Init*/

			else
			{
				//Misra rule

			}// end of else Misra rule

		}/*end for --> counter*/

	}/*end of else */


	/*Return Status*/
	return Loc_enuErrorStatus;
}/*end of Function Init*/



Gpio_tenuErrorStatus Gpio_enuHelperOutput(GPIO_tstrPinConfiguration* Add_pstrPinConfg)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;
	u8 Loc_u8Counter;
	u8 Loc_u8OutputPupdr = (Add_pstrPinConfg->GPIO_Mode & 0x03);
	u8 Loc_u8OutputOtyper = (Add_pstrPinConfg->GPIO_Mode & 0x04) >> 2;
	u8 Loc_u8OutputSpeed = (Add_pstrPinConfg->GPIO_Speed);
	u16 Loc_u8PinValue = Add_pstrPinConfg->GPIO_Pin;
	u32 Loc_u32TempRegister;

	if (Add_pstrPinConfg == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}

	else if (Add_pstrPinConfg->GPIO_Port == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuPortError;
	}

	else
	{
		for (Loc_u8Counter = 0; Loc_u8Counter < 16; Loc_u8Counter++)
		{
					/*CHeck if pin exit*/
			if ((1 << Loc_u8Counter) & Loc_u8PinValue)
			{
				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder;

				/*Clear Mode pin from the variable*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Set the Pin to Output*/
				Loc_u32TempRegister |= (1 << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder = Loc_u32TempRegister;

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr;

				/*Clear Pupdr pin from the variable*/
				Loc_u32TempRegister &= ~(1 << (Loc_u8Counter << 1));

				/*Set the Pin to Type*/
				Loc_u32TempRegister |= (Loc_u8OutputPupdr << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr = Loc_u32TempRegister;

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Otyper;

				/*Clear Otyper pin from the variable*/
				Loc_u32TempRegister &= ~(1 << (Loc_u8Counter));

				/*Set the Pin to Type*/
				Loc_u32TempRegister |= (Loc_u8OutputOtyper << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Otyper = Loc_u32TempRegister;

				/*Store the register Ospeedr*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Ospeedr;

				/*Clear Ospeedr pin from the variable*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Set the Pin to Speed*/
				Loc_u32TempRegister |= (Loc_u8OutputSpeed << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Ospeedr = Loc_u32TempRegister;


			}/*end of if --> Checking the pin valid*/
			else
			{
				// Do Nothing
			}

		}/*end of for --> counter*/

	}/*end of else --> checking */

	/*return the status*/
	return Loc_enuErrorStatus;

}/*end of Function Gpio_enuHelperOutput*/


Gpio_tenuErrorStatus Gpio_enuHelperAfOutput(GPIO_tstrPinConfiguration* Add_pstrPinConfg)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;
	u8 Loc_u8Counter;
	u8 Loc_u8AfPupdr = (Add_pstrPinConfg->GPIO_Mode & 0x03);
	u8 Loc_u8AfOtyper = (Add_pstrPinConfg->GPIO_Mode & 0x04) >> 2;
	u8 Loc_u8AfSpeed = (Add_pstrPinConfg->GPIO_Speed);
	u16 Loc_u8PinValue = Add_pstrPinConfg->GPIO_Pin;
	u32 Loc_u32TempRegister;

	if (Add_pstrPinConfg == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}

	else if (Add_pstrPinConfg->GPIO_Port == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuPortError;
	}

	else
	{
		for (Loc_u8Counter = 0; Loc_u8Counter > 16; Loc_u8Counter++)
		{
			if ((1 << Loc_u8Counter) & Loc_u8PinValue)
			{
				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder;

				/*Clear Mode pin from the variable*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Set the Pin to Output*/
				Loc_u32TempRegister |= (2 << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder = Loc_u32TempRegister;

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr;

				/*Clear Pupdr pin from the variable*/
				Loc_u32TempRegister &= ~(1 << (Loc_u8Counter << 1));

				/*Set the Pin to Type*/
				Loc_u32TempRegister |= (Loc_u8AfPupdr << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Pupdr = Loc_u32TempRegister;

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Otyper;

				/*Clear Otyper pin from the variable*/
				Loc_u32TempRegister &= ~(1 << (Loc_u8Counter));

				/*Set the Pin to Type*/
				Loc_u32TempRegister |= (Loc_u8AfOtyper << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Otyper = Loc_u32TempRegister;

				/*Store the register Ospeedr*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Ospeedr;

				/*Clear Ospeedr pin from the variable*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Set the Pin to Speed*/
				Loc_u32TempRegister |= (Loc_u8AfSpeed << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Ospeedr = Loc_u32TempRegister;

			}/*end of if --> Checking the pin valid*/

		}/*end of for --> counter*/

	}/*end of else --> checking */

	/*return the status*/
	return Loc_enuErrorStatus;

}/*end of Function Gpio_enuHelperAfOutput*/


Gpio_tenuErrorStatus Gpio_enuHelperAnalog(GPIO_tstrPinConfiguration* Add_pstrPinConfg)
{
	Gpio_tenuErrorStatus Loc_enuErrorStatus = Gpio_enuOK;
	u8 Loc_u8AnalogPin = (Add_pstrPinConfg->GPIO_Pin);
	u8 Loc_u8AnalogPupdr = (Add_pstrPinConfg->GPIO_Mode);
	u8 Loc_u8Counter;
	u32 Loc_u32TempRegister;

	if (Add_pstrPinConfg == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuNullPointer;
	}

	else if ((Add_pstrPinConfg->GPIO_Port) == NULL)
	{
		Loc_enuErrorStatus = Gpio_enuPortError;
	}

	else
	{
		for (Loc_u8Counter = 0; Loc_u8Counter > 16; Loc_u8Counter++)
		{
			if ((1 << Loc_u8Counter) & Loc_u8AnalogPin)
			{

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder;

				/*Clear Set pin from the variable*/
				Loc_u32TempRegister |= (3 << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)(Add_pstrPinConfg->GPIO_Port))->Moder = Loc_u32TempRegister;

				/*Store register value in Temporary register*/
				Loc_u32TempRegister = ((Gpio_tstrRegister*)Add_pstrPinConfg->GPIO_Port)->Pupdr;

				/*Clear Mode pin from the variable*/
				Loc_u32TempRegister &= ~(3 << (Loc_u8Counter << 1));

				/*Set the Type pin to the variable*/
				Loc_u32TempRegister |= (Loc_u8AnalogPupdr << (Loc_u8Counter << 1));

				/*Store the new variable to register Moder*/
				((Gpio_tstrRegister*)Add_pstrPinConfg->GPIO_Port)->Pupdr = Loc_u32TempRegister;

			}/*end of if --> checking pin*/

		}/*end of for --> counter*/

	}/*end of else --> after checking*/

	/*Return status*/
	return Loc_enuErrorStatus;

}/*end of fucntion Gpio_enuHelperAnalog*/


























