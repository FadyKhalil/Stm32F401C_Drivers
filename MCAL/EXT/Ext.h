/*
 * Ext.h
 *
 *  Created on: Apr 19, 2022
 *      Author: fady
 */

#ifndef MCAL_EXT_EXT_H_
#define MCAL_EXT_EXT_H_


typedef void(*EXTI_Cbf)(void);

typedef enum
{
	EXT0 = 0,
	EXT1,
	EXT2,
}Channel_t;

typedef enum
{
	INTERRUPT_MASKABLE,
	EVENT_MASKABLE

}Mode_t;

typedef enum
{
	PINA = 0,
	PINB,
	PINC,
	PIND,
	PINE,
	PINH

}Pin_t;

typedef enum
{
	DISABLE = 0,
	ENABLE

}State_t;

typedef struct
{
	Channel_t Channel;
	Mode_t Mode;
	Pin_t Pin;
	State_t Interrupt_Condition;
	State_t TriggerFalling;
	State_t TriggerRising;

}External_Config_t;


typedef struct
{
	Channel_t Channel;
	u8 Software_Int_State;

}External_Software_t;


extern void External_vidInit(External_Config_t* ConfigPtr);

extern void External_vidEnable(u8 Copy_u8ExtLine);

extern void External_vidDisable(u8 Copy_u8ExtLine);

extern void External_vidSoftwareInt(External_Software_t* ConfigPtr);

extern void External_vidDisablePending(u8 Copy_u8PendingLine);

extern void External_vidRegisterCallBack(EXTI_Cbf CallBack, Channel_t Channel);

#endif /* MCAL_EXT_EXT_H_ */
