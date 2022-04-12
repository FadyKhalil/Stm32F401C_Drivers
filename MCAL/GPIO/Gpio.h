/*
 * Gpio.h
 *
 *  Created on: Mar 16, 2022
 *      Author: fady
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "../Services/Std_types.h"

/*##########################################################################*/
/**************************Types**************************/


/*Typedef for Error Status
 *
 * @Return:
 * 			Gpio_enuOk 			--> Configuration done correctly
 * 			Gpio_enuPortError	--> Port Chosen Wrong
 * 			Gpio_enuPinError	-->	Pin Chosen Wrong
 * 			Gpio_enuNullPointer	--> Pointer to NULL
 * */
typedef enum
{

	Gpio_enuOK = 0,
	Gpio_enuPortError,
	Gpio_enuPinNumberError,
	Gpio_enuNullPointer,
	Gpio_enuPinValueError


}Gpio_tenuErrorStatus;


/*Typedef For Gpio Configuration
 *
 * @ Parameter
 *
 * Gpio_Port	--> Correspond to the port chosen for configuration
 *
 * Gpio_Mode	--> For Mode Option for the pin usage
 * 				!! Refer to the section bellow for mode configuration
 *
 * Gpio_Speed	--> Speed of the PIN
 *
 * Gpio_Pin		--> Pin in Range {0 -> 15} for each port
 *
 * */
typedef struct
{
	void * GPIO_Port;
	u8 GPIO_Mode;
	u8 GPIO_Speed;
	u16 GPIO_Pin;

}GPIO_tstrPinConfiguration;

/*#########################################################################*/



/*#########################################################################*/
/*************************Defines*************************/

/*Parameter to be used for function:
 * @ Gpio_PinModeConfiguration
 */
#define GPIO_A	((void*)0x40020000)
#define GPIO_B	((void*)0x40020400)
#define GPIO_C	((void*)0x40020800)
#define GPIO_D	((void*)0x40020C00)
#define GPIO_E	((void*)0x40021000)
#define GPIO_H	((void*)0x40021C00)

/*Parameter to be used for function:
 * @ Gpio_PinModeConfiguration
 * */

#define GPIO_PIN00			0x0001
#define GPIO_PIN01			0x0002
#define GPIO_PIN02			0x0004
#define GPIO_PIN03			0x0008
#define GPIO_PIN04			0x0010
#define GPIO_PIN05			0x0020
#define GPIO_PIN06			0x0040
#define GPIO_PIN07			0x0080
#define GPIO_PIN08			0x0100
#define GPIO_PIN09			0x0200
#define GPIO_PIN10			0x0400
#define GPIO_PIN11			0x0800
#define GPIO_PIN12			0x1000
#define GPIO_PIN13			0x2000
#define GPIO_PIN14			0x4000
#define GPIO_PIN15			0x8000



/*Parameter to be used for function:
 * @ Gpio_PinModeConfiguration
 *
 * ---> To set the Pin Mode
 */

#define DELETE_MAGIC							0xEFFFFFFF

#define GPIO_u8INPUT_FLOATING					0b00000
#define GPIO_u8INPUT_PULLUP						0b00001
#define GPIO_u8INPUT_PULLDOWN					0b00010

#define GPIO_u8OUTPUT_PUSHPULL					0b01000
#define GPIO_u8OUTPUT_PUSHPULL_PULLUP			0b01001
#define GPIO_u8OUTPUT_PUSHPULL_PULLDOWN			0b01010
#define GPIO_u8OUTPUT_OPENDRAIN					0b01100
#define GPIO_u8OUTPUT_OPENDRAIN_PULLUP			0b01101
#define GPIO_u8OUTPUT_OPENDRAIN_PULLDOWN		0b01110

#define GPIO_u8ALTFUNC_PUSHPULL					0b10000
#define GPIO_u8ALTFUNC_PUSHPULL_PULLUP			0b10001
#define GPIO_u8ALTFUNC_PUSHPULL_PULLDOWN		0b10010
#define GPIO_u8ALTFUNC_OPENDRAIN				0b10100
#define GPIO_u8ALTFUNC_OPENDRAIN_PULLUP			0b10101
#define GPIO_u8ALTFUNC_OPENDRAIN_PULLDOWN		0b10110

#define GPIO_u8INPUT_OUTPUT_ANALOG				0b11000


/*Parameter to be used for function:
 * @ Gpio_PinModeConfiguration
 *
 * ---> To set the value of pin
 */
#define GPIO_u8HIGH	1
#define GPIO_u8LOW	0


/*Parameter to be used for function:
 * @ Gpio_PinModeConfiguration
 *
 * ---> To set the pin speed
 */
#define GPIO_Speed_Low				0x00
#define GPIO_Speed_Medium			0x01
#define GPIO_Speed_High				0x10
#define GPIO_Speed_Very_High		0x11

/*#####################################################################*/



/************************************************************/
/******************Function prototype************************/



/*
 * Public Function: Gpio_PinModeConfiguration
 *
 * Description: 		Function to configure the pin mode such as:
 * 						@ INPUT OR OUTPUT
 * 						@ PUSHPULL OR OVERDRAIN
 * 						@ PULL-UP OR PULL-DOWN
 *
 *
 * Input Parameter:		This Function take three input
 *
 * 						@ PORT:
 * 						--> GPIO_A
 * 						--> GPIO_B
 * 						--> GPOI_C
 * 						--> GPIO_D
 * 						--> GPIO_E
 * 						--> GPIO_H
 *
 * 						@ PinNumber
 *
 * 						@ Pin Configuration
 * 						--> GPIO_u8INPUT_FLOATING
						-->	GPIO_u8INPUT_PULLUP
						-->	GPIO_u8INPUT_PULLDOWN
						-->	GPIO_u8OUTPUT_PUSHPULL
						-->	GPIO_u8OUTPUT_PUSHPULL_PULLUP
						-->	GPIO_u8OUTPUT_PUSHPULL_PULLDOWN
						-->	GPIO_u8OUTPUT_OPENDRAIN
						-->	GPIO_u8OUTPUT_OPENDRAIN_PULLUP
						-->	GPIO_u8OUTPUT_OPENDRAIN_PULLDOWN
						-->	GPIO_u8ALTFUNC_PUSHPULL
						-->	GPIO_u8ALTFUNC_PUSHPULL_PULLUP
						-->	GPIO_u8ALTFUNC_PUSHPULL_PULLDOWN
						-->	GPIO_u8ALTFUNC_OPENDRAIN
						-->	GPIO_u8ALTFUNC_OPENDRAIN_PULLUP
						-->	GPIO_u8ALTFUNC_OPENDRAIN_PULLDOWN


 *
 * Return:				Return Error Status if any
 *
 * */
Gpio_tenuErrorStatus Gpio_enuPinConfigurationInit(GPIO_tstrPinConfiguration* Add_pstrPinConfg);



/*
 * Public Function: 	Gpio_SetPinValue
 *
 * Description: 		Function to set or reset pin:
 * 						@ INPUT OR OUTPUT
 * 						@ PUSHPULL OR OVERDRAIN
 * 						@ PULL-UP OR PULL-DOWN
 *
 *
 * Input Parameter:		This Function take three input
 *
 * 						@ Port:
 * 						--> GPIOA
 * 						--> GPIOB
 * 						--> GPOIC
 * 						--> GPIOD
 * 						--> GPIOE
 * 						--> GPIOH
 *
 * 						@ Copy_u8PinNumber
 *
 * 						@ Value:
 * 						--> GPIO_u8LOW
						-->	GPIO_u8HIGH

 *
 * Return:				Return Error Status if any
 *
 * */
Gpio_tenuErrorStatus Gpio_enuSetPinValue(void* Add_vidGpioPort, u16 Copy_u8GpioPinNumber, u8 Copy_u8GpioPinValue);


/*
 * Public Function: 	Gpio_GetPinValue
 *
 * Description: 		Function to set or reset pin:
 * 						@ INPUT OR OUTPUT
 * 						@ PUSHPULL OR OVERDRAIN
 * 						@ PULL-UP OR PULL-DOWN
 *
 *
 * Input Parameter:		This Function take three input
 *
 * 						@ Port:
 * 						--> GPIO_A
 * 						--> GPIO_B
 * 						--> GPOI_C
 * 						--> GPIO_D
 * 						--> GPIO_E
 * 						--> GPIO_H
 *
 * 						@ Copy_u8PinNumber:
 *
 * 						@ Add_pu8PinValue:
 * 						--> Variable to get the value
 *

 *
 * Return:				Return Error Status if any
 *
 * */
Gpio_tenuErrorStatus Gpio_enuGetPinValue(void* Add_vidGpioPort, u16 Copy_u8GpioPinNumber, pu8 Add_pu8GpioPinValue);


#endif
