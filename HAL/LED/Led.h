/*
 * Led.h
 *
 *  Created on: Mar 27, 2022
 *      Author: fady
 */

#ifndef LED_H_
#define LED_H_

#include "Std_types.h"
#include "Led_cfg.h"

/*################################################################*/
/******************************Types******************************/

typedef enum
{
	Led_enuOk=0,
	Led_enuPinError

}Led_tenuErrorStatus;


typedef struct
{
	u32* Port;
	u16 Pin;
	u8 Active_states;
	u8 Otype;
	u8 Speed;

}Led_tstrConfiguration;

/*###############################################################*/


/*###############################################################*/
/****************************Defines*****************************/

/*Type of Led
 * @ Push Pull 	(PP)
 * @ Open Drain (OD)
 * */
#define LED_OTYPE_PP	0
#define LED_OTYPE_OD	1

/*Port Connected*/
#define PORTA	GPIO_A
#define PORTB	GPIO_B
#define PORTC	GPIO_C
#define PORTD	GPIO_D
#define PORTE	GPIO_E
#define PORTH	GPIO_H


/*Led Init Status
 * @ Active
 * @ Close
 * */
#define LED_ACTIVE		0
#define LED_CLOSE		1

/*Led Speed Pin*/
#define LED_LOW_SPEED 		GPIO_Speed_Low
#define LED_MED_SPEED 		GPIO_Speed_Medium
#define LED_HIGH_SPEED 		GPIO_Speed_High
#define LED_VERYHIGH_SPEED 	GPIO_Speed_Very_High_

/*################################################################*/


/*###############################################################*/
/*********************Function Protype***************************/

extern Led_tenuErrorStatus Led_enuInit(void);


extern Led_tenuErrorStatus Led_enuStatus(u16 Copy_u16LedNumber, u8 Copy_u8LedStatus);

/*##############################################################*/


#endif /* LED_H_ */
