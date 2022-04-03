/*
 * Nvic.h
 *
 *  Created on: Mar 28, 2022
 *      Author: fady
 */

#include "../../../Services/Std_types.h"
#include "Nvic.h"



typedef struct
{

	u32 NVIC_ISER[8];

	u32 Reserved[24];

	u32 NVIC_ICER[8];

	u32 Reserved1[24];

	u32 NVIC_ISPR[8];

	u32 Reserved2[24];

	u32 NVIC_ICPR[8];

	u32 Reserved3[24];

	u32 NVIC_IABR[8];

	u32 Reserved4[56];

	u32 NVIC_IPR[59];

	u32 Reserved5[643];

	u32 NVIC_STIR;

}Nvic_tstrRegister;


#define NVIC 	((Nvic_tstrRegister*)(0xE000E100))


/*####################################################################################*/
/*********************************Function Implementation******************************/

void Nvic_vidEnableIrq(u8 Copy_strIrq)
{
	NVIC->NVIC_ISER[Copy_strIrq/32] |= 1 << (Copy_strIrq%32);

}

void Nvic_vidDisableIrq(u8 Copy_strIrq)
{
	NVIC->NVIC_ICER[Copy_strIrq/32] |= 1 << (Copy_strIrq%32);
}


void Nvic_vidSetPendingIrq(u8 Copy_strIrq)
{
	NVIC->NVIC_ISPR[Copy_strIrq/32] |= 1 << (Copy_strIrq%32);
}


void Nvic_vidClearPendingIrq(u8 Copy_strIrq)
{
	NVIC->NVIC_ICPR[Copy_strIrq/32] |= 1 << (Copy_strIrq%32);
}

void Nvic_vidGetPendingIrq(u8 Copy_strIrq, pu8 Add_pu8PendingStatus)
{
	*Add_pu8PendingStatus = (NVIC->NVIC_IABR[Copy_strIrq/32] >> (Copy_strIrq%32)) & 0x01;
}

void Nvic_vidSetPriority(u8 Copy_strIrq, u8 Copy_u8Prioty)
{

	NVIC->NVIC_IPR[Copy_strIrq/4] = Copy_u8Prioty << ((Copy_strIrq%4) << 3);

}





