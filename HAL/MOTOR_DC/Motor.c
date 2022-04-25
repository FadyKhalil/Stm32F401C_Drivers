/*
 * Motor.c
 *
 *  Created on: Apr 5, 2022
 *      Author: fady
 */


#include "../../Services/Std_types.h"
#include "../../MCAL/RCC/Rcc.h"
#include "../../MCAL/GPIO/Gpio.h"
#include "Motor_cfg.h"
#include "Motor.h"


extern Motor_tstrConfiguration Motor_strMotors[];

Motor_tenuErrorStatus Motor_enuInit(void)
{
	Motor_tenuErrorStatus Loc_enuErrorStatus = Motor_enuOK;
	u8 Loc_u8Counter = 0, Loc_u8CheckingClockON;
	/*Create object from gpio configuration*/
	GPIO_tstrPinConfiguration Gpio_Configuration;

	/*For loop to loop around the pin to init*/
	for(Loc_u8Counter = 0; Loc_u8Counter < NUMBER_OF_MOTOR; Loc_u8Counter++)
	{
		if (Motor_strMotors[Loc_u8Counter].Otype == MOTOR_OTYPE_PP)
		{
			Gpio_Configuration.GPIO_Mode = GPIO_u8OUTPUT_PUSHPULL;
		}
		else
		{
			Gpio_Configuration.GPIO_Mode = GPIO_u8OUTPUT_OPENDRAIN;
		}

		/*Init the Pin number to the gpio*/
		Gpio_Configuration.GPIO_Pin = Motor_strMotors[Loc_u8Counter].Pin;
		/*Init the Port number to the gpio*/
		Gpio_Configuration.GPIO_Port = Motor_strMotors[Loc_u8Counter].Port;
		/*Init the Pin speed*/
		Gpio_Configuration.GPIO_Speed = Motor_strMotors[Loc_u8Counter].Speed;

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



Motor_tenuErrorStatus Motor_enuMotorState(u8 Copy_u8MotorPin, u8 Copy_u8MotorState)
{
	Motor_tenuErrorStatus Loc_enuErrorStatus= Motor_enuOK;


		/*Set Pin value
		 *
		 * ---> Getting the Port Number by the user for the configuration .c file
		 *
		 * ---> Getting number of pin by the user array
		 *
		 * ---> Setting the status by using XOR in case of active state
		 *
		 * */
		Loc_enuErrorStatus = Gpio_enuSetPinValue(Motor_strMotors[Copy_u8MotorPin].Port, Motor_strMotors[Copy_u8MotorPin].Pin,
										Copy_u8MotorState ^ Motor_strMotors[Copy_u8MotorPin].Active_states);

		/*return status*/
		return Loc_enuErrorStatus;
}
