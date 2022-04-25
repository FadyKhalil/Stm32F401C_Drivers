/*
 * Led.h
 *
 *  Created on: Mar 27, 2022
 *      Author: fady
 */

#ifndef BUZ_H_
#define BUZ_H_

#include "Buz_cfg.h"

/*################################################################*/
/******************************Types******************************/

typedef enum
{
	Buz_enuOk=0,
	Buz_enuPinError

}Buz_tenuErrorStatus;


typedef struct
{
	u32* Port;
	u16 Pin;
	u8 Active_states;
	u8 Otype;
	u8 Speed;

}Buz_tstrConfiguration;

/*###############################################################*/


/*###############################################################*/
/****************************Defines*****************************/

/*Type of Led
 * @ Push Pull 	(PP)
 * @ Open Drain (OD)
 * */
#define BUZ_OTYPE_PP	0
#define BUZ_OTYPE_OD	1

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
#define BUZ_ACTIVE		0
#define BUZ_CLOSE		1

/*Led Speed Pin*/
#define BUZ_LOW_SPEED 		GPIO_Speed_Low
#define BUZ_MED_SPEED 		GPIO_Speed_Medium
#define BUZ_HIGH_SPEED 		GPIO_Speed_High
#define BUZ_VERYHIGH_SPEED 	GPIO_Speed_Very_High_

/*################################################################*/


/*###############################################################*/
/*********************Function Protype***************************/

extern Buz_tenuErrorStatus Buz_enuInit(void);


extern Buz_tenuErrorStatus Buz_enuStatus(u16 Copy_u16BuzNumber, u8 Copy_u8BuzStatus);

/*##############################################################*/


#endif /* LED_H_ */
