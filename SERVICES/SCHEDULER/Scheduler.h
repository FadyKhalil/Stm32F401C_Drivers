/*
 * Schefuler.h
 *
 *  Created on: Apr 2, 2022
 *      Author: fady
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Std_types.h"



typedef enum
{
	Sched_enuOk = 0,
	Sched_enuError

}Sched_tenuErrorStatus;


typedef struct
{
	void(*CallBack_t)(void);
	u32 CyclicTime_ms;

}Sched_tstrRunnable;



/*Public function:	Sched_vidRegisterRunnable
 *
 *
 * Description:		This function take the number of task in the application
 * */
extern Sched_tenuErrorStatus Sched_vidRegisterRunnable(Sched_tstrRunnable Copy_strRunnable, u32 Copy_u8Priority);


/*
 * Public Function: Sched_vidInit
 *
 *
 * Description:		Initialize the scheduler with the systick time
 *
 * */
extern void Sched_vidInit(void);
extern void Sched_vidStart(void);


#endif /* SCHEDULER_H_ */
