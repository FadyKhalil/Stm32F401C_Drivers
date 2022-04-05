/*
 * Motor.h
 *
 *  Created on: Apr 5, 2022
 *      Author: fady
 */

#ifndef MOTOR_H_
#define MOTOR_H_

typedef enum
{
	Motor_enuOK = 0,
	Motor_enuNok

}Motor_tenuErrorStatus;

typedef struct
{
	void* Port;
	u16 Pin;
	u8 Active_states;
	u8 Otype;
	u8 Speed;

}Motor_tstrConfiguration;

/*###############################################################*/
/****************************Defines*****************************/

/*Type of Led
 * @ Push Pull 	(PP)
 * @ Open Drain (OD)
 * */
#define MOTOR_OTYPE_PP	0
#define MOTOR_OTYPE_OD	1

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
#define MOTOR_ACTIVE		0
#define MOTOR_CLOSE		1

/*Led Speed Pin*/
#define MOTOR_LOW_SPEED 		GPIO_Speed_Low
#define MOTOR_MED_SPEED 		GPIO_Speed_Medium
#define MOTOR_HIGH_SPEED 		GPIO_Speed_High
#define MOTOR_VERYHIGH_SPEED 	GPIO_Speed_Very_High_

/*################################################################*/



Motor_tenuErrorStatus Motor_enuInit(Motor_tstrConfiguration Copy_strConfiguration);


Motor_tenuErrorStatus Motor_enuMotorState(u8 Copy_u8MotorState);





#endif /* MOTOR_H_ */
