/*
 * Switch.h
 *
 *  Created on: Apr 4, 2022
 *      Author: fady
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "Switch_cfg.h"

typedef enum
{
	Switch_enuOk = 0,
	Switch_enuWrongPin,
	Switch_enuNullPointer

}Switch_tenuErrorStatus;



typedef struct
{
	void* Port;
	u16 Pin;
	u8 Mode;

}Switch_tstrConfiguration;


#define SWITCH_PULLUP		GPIO_u8INPUT_PULLUP
#define SWITCH_PULLDOWN		GPIO_u8INPUT_PULLDOWN
#define SWITCH_FLOATIN		GPIO_u8INPUT_FLOATING


#define PORTA	GPIO_A
#define PORTB	GPIO_B
#define PORTC	GPIO_C
#define PORTD	GPIO_D
#define PORTE	GPIO_E
#define PORTH	GPIO_H



extern void Switch_vidTask(void);
extern Switch_tenuErrorStatus Switch_vidInit(void);
extern Switch_tenuErrorStatus Switch_enuReadStatus(u8 Copy_u8SwitchNumber, pu8 Add_pu8PinStatus);


#endif /* SWITCH_H_ */
