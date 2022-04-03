/*
 * Scheduler.c
 *
 *  Created on: Apr 2, 2022
 *      Author: fady
 */

#include "Std_types.h"
#include "Sched_cfg.h"
#include "Scheduler.h"
#include "Systick.h"



static u8 Sched_prvStartSched = 0;

static Sched_tstrRunnable runnableList[MAX];

#define Tick_ms		1


static void Sched_prvTask(void);
static void Sched_vidTickCbf(void);

Sched_tenuErrorStatus Sched_vidRegisterRunnable(Sched_tstrRunnable Copy_strRunnable, u32 Copy_u8Priority)
{
	Sched_tenuErrorStatus Loc_enuErrorStatus = Sched_enuOk;

	if(runnableList[Copy_u8Priority].CallBack_t == 0)
	{
		runnableList[Copy_u8Priority] = Copy_strRunnable;
	}
	else
	{
		Loc_enuErrorStatus = Sched_enuError;
	}

	return Loc_enuErrorStatus;
}

void Sched_vidInit(void)
{
	Systick_vidInit(SYSTICK_PROCESSORCLK_INTERUPT);
	Systick_vidSetTime(Tick_ms);
	Systick_vidRegisterCallBack(Sched_vidTickCbf);
}


void Sched_vidStart(void)
{

	Systick_vidStart();

	while(1)
	{
		if(Sched_prvStartSched == 1)
		{
			Sched_prvStartSched = 0;
			Sched_prvTask();
		}
	}

}


static void Sched_vidTickCbf(void)
{
	Sched_prvStartSched = 1;
	return ;
}


static void Sched_prvTask(void)
{
	u8 Loc_u8Counter;
	static u32 Loc_u32prvTimeMs =  0;

	for(Loc_u8Counter = 0; Loc_u8Counter < MAX; Loc_u8Counter++)
	{
		if((Loc_u32prvTimeMs % (runnableList[Loc_u8Counter].CyclicTime_ms)) == 0)
		{
			(runnableList[Loc_u8Counter].CallBack_t)();
		}
	}

	Loc_u32prvTimeMs += Tick_ms;

	return;
}
