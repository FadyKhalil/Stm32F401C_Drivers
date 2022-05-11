/*
 * Fprog.c
 *
 *  Created on: May 10, 2022
 *      Author: fady
 */

#include "../../Services/Std_types.h"
#include "Fprog.h"

/*to initialize the register memory for flash driver*/
typedef struct {
	u32 ACR;
	u32 KEYR;
	u32 OPTKEYR;
	u32 SR;
	u32 CR;
	u32 OPTCR;

} Fprog_Reg;

typedef struct {
	u8 _Index_[512];
	u8 _LOCK_[16];

} OTP_Reg;

/***************Define******************/
/*Memory Map Address for Flash Driver*/
#define FLASH	((volatile Fprog_Reg*)0x40023C00)
/*Memory Map One time Programmable byte*/
#define OTP		((volatile OTP_Reg*)0x1FFF7800)

/*Maximum sector number to be chose*/
#define MAX_SECTOR_NUMBER	6

/*Key Unlock the Control Register*/
#define KEY_UNLOCK_1	0x45670123
#define KEY_UNLOCK_2	0xCDEF89AB

#define SR_BUSY_BIT			16
#define CR_LOCK_BIT			31

/*Programing Flash*/
#define CR_PG				0
/*Sectro Erase Bit*/
#define CR_SER				1
/*Mass Erase Bit*/
#define CR_MER				2
/*Sector Number*/
#define CR_SNB				3
/*Page size*/
#define CR_PSIZE_BIT		8
/*Start Erase Bit*/
#define CR_START			16
/*end of operation bit*/
#define CR_EOPIE			24
/*error inturrupt bit*/
#define CR_ERRIE			25
/*Lock Bit */
#define CR_LOCK_BIT			31

/*Function Like Macro*/

/*Lock the Flash driver*/
#define LOCK_FLASH_ACTION	(FLASH->CR |= 1 << CR_LOCK_BIT)

void Fprog_vidInit(const Fprog_Config* ConfigPtr) {
	u8 Loc_u8TimerOut = 0;
	if ((FLASH->SR >> SR_BUSY_BIT) == 1) {
		/*Flashing on progress*/
	}/*end of if*/
	else if (!ConfigPtr) {
		/*Null Pointer*/
	}/*end of else if*/
	else {
		/*Unlock control register*/
		FLASH->KEYR = KEY_UNLOCK_1;
		FLASH->KEYR = KEY_UNLOCK_2;

		/*Check if unlock*/
		while (FLASH->CR >> CR_LOCK_BIT && Loc_u8TimerOut < 20) {
			Loc_u8TimerOut++;
		}/*end of while*/
		if (Loc_u8TimerOut == 20) {
			/*The register is still lock*/

		}/*end of if*/
		else {

			/*Flash memmory corespond to the CPU Clock*/
			FLASH->ACR |= ConfigPtr->FlashAccessLatency;

			/*configuration of Interrupt*/
			FLASH->CR |= ConfigPtr->Interrupt.EndOfOperation << CR_EOPIE
					| ConfigPtr->Interrupt.Error << CR_ERRIE;

		}/*end of else*/

	}/*end of main else*/

	LOCK_FLASH_ACTION;

}/*end of function Fprog_vidInit*/

Fprog_tenuErrorStatus Fprog_enuMassErase(void) {
	u8 Loc_u8TimerOut = 0;
	Fprog_tenuErrorStatus Loc_enuErrorStatus = Fpro_enuOK;
	while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
		/*Flashing on progress*/
	}/*end of while*/

	/*Unlock control register*/
	FLASH->KEYR = KEY_UNLOCK_1;
	FLASH->KEYR = KEY_UNLOCK_2;

	/*Check if unlock*/
	while (FLASH->CR >> CR_LOCK_BIT != 0 && Loc_u8TimerOut < 20) {
		Loc_u8TimerOut++;
	}/*end of while*/
	if (Loc_u8TimerOut == 20) {
		/*The register is still lock*/
		Loc_enuErrorStatus = Fprog_enuControlRegisterLock;

	}/*end of if*/
	else {
		/*Enable Mass erasing*/
		FLASH->CR |= 1 << CR_MER;
		/*Start Erasing*/
		FLASH->CR |= 1 << CR_START;

		/*Wait to Flash end*/
		while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
			/*Flashing on progress*/
		}/*end of while*/
	}

	LOCK_FLASH_ACTION;

	return Loc_enuErrorStatus;
}

Fprog_tenuErrorStatus Fprog_enuSectorErase(u8 SectorNumber) {

	u32 Loc_u32TempRegister;
	u8 Loc_u8TimerOut = 0;
	Fprog_tenuErrorStatus Loc_enuErrorStatus = Fpro_enuOK;
	if (SectorNumber > MAX_SECTOR_NUMBER) {
		Loc_enuErrorStatus = Fprog_enuSectorNumberWrong;
	}
	else
	{
		while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
			/*Flashing on progress*/
		}/*end of while*/

		/*Unlock control register*/
		FLASH->KEYR = KEY_UNLOCK_1;
		FLASH->KEYR = KEY_UNLOCK_2;

		/*Check if unlock*/
		while (FLASH->CR >> CR_LOCK_BIT != 0 && Loc_u8TimerOut < 20) {
			Loc_u8TimerOut++;
		}/*end of while*/
		if (Loc_u8TimerOut == 20) {
			/*The register is still lock*/
			Loc_enuErrorStatus = Fprog_enuControlRegisterLock;

		}/*end of if*/

		else {

			/*Read register value*/
			Loc_u32TempRegister = FLASH->CR;

			/*Clear sector bits*/
			Loc_u32TempRegister &= ~(7 << CR_SNB);

			/*Enable Sector erase and Select the Sector*/
			Loc_u32TempRegister |= 1 << CR_SER | SectorNumber << CR_SNB;

			/*Assign the register value*/
			FLASH->CR = Loc_u32TempRegister;

			/*Start Erasing*/
			FLASH->CR |= 1 << CR_START;

			/*Wait to Flash end*/
			while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
				/*Flashing on progress*/
			}/*end of while*/

		}

		LOCK_FLASH_ACTION;
	}

	return Loc_enuErrorStatus;

}

Fprog_tenuErrorStatus Fprog_enuOneTimeProgramming(pu32 Data, u8 BufferSize) {

	static u8 Loc_u8Counter;
	u8 Loc_u8TimerOut = 0;
	u8 Loc_u8Size = 0;

	Fprog_tenuErrorStatus Loc_enuErrorStatus = Fpro_enuOK;
	while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
		/*Flashing on progress*/
	}/*end of while*/

	/*Unlock control register*/
	FLASH->KEYR = KEY_UNLOCK_1;
	FLASH->KEYR = KEY_UNLOCK_2;

	/*Check if unlock*/
	while (FLASH->CR >> CR_LOCK_BIT != 0 && Loc_u8TimerOut < 20) {
		Loc_u8TimerOut++;
	}/*end of while*/
	if (Loc_u8TimerOut == 20) {
		/*The register is still lock*/
		Loc_enuErrorStatus = Fprog_enuControlRegisterLock;

	}/*end of if*/

	else {
		/*enable Programing*/
		FLASH->CR |= 1 << CR_PG;

		/*Loop around the buffer*/
		while (Loc_u8Size < BufferSize) {
			/*Store the data to the OTP register*/
			OTP->_Index_[Loc_u8Counter] = Data[Loc_u8Size];

			/*lock One time Programmable Byte (OTP)*/
			if ((Loc_u8Counter + 1) % 8 == 0) {
				OTP->_LOCK_[Loc_u8Counter % 4];
			} else {
				//Do nothing
			}

			/*Incrementing the Counter && Bufer Size*/
			Loc_u8Counter++;
			Loc_u8Size++;
		}

	}

	LOCK_FLASH_ACTION;

	return Loc_enuErrorStatus;
}

Fprog_tenuErrorStatus Fprog_enuProgrammingFlash(u64 Data, u32 MemoryAddress, u8 Copy_u8Pagesize) {

	u8 Loc_u8TimerOut = 0;
	u32 Loc_u32TempRegister = 0;
	Fprog_tenuErrorStatus Loc_enuErrorStatus = Fpro_enuOK;

	if (MemoryAddress > 0x0803FFFF || MemoryAddress < 0x08000000) {
		Loc_enuErrorStatus = Fprog_enuExceedFlash;
	}
	else
	{

		while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
			/*Flashing on progress*/
		}/*end of while*/

		/*Unlock control register*/
		FLASH->KEYR = KEY_UNLOCK_1;
		FLASH->KEYR = KEY_UNLOCK_2;

		/*Check if unlock*/
		while (FLASH->CR >> CR_LOCK_BIT != 0 && Loc_u8TimerOut < 20) {
			Loc_u8TimerOut++;
		}/*end of while*/
		if (Loc_u8TimerOut == 20) {
			/*The register is still lock*/
			Loc_enuErrorStatus = Fprog_enuControlRegisterLock;

		}/*end of if*/

		else {

			/*Load value of Control register to variable*/
			Loc_u32TempRegister = FLASH->CR;

			/*enable Programing & Configure the page size programming*/
			Loc_u32TempRegister |= 1 << CR_PG | Copy_u8Pagesize << CR_PSIZE_BIT;

			FLASH->CR |= Loc_u32TempRegister;

			/*Prgramming the Flash Memory*/
			switch(Copy_u8Pagesize)
			{

			case PAGESIZE_BYTE:
				(*(volatile u32*)MemoryAddress) = (u8)Data;
				break;

			case PAGESIZE_HALF_WORD:
				(*(volatile u32*)MemoryAddress) = (u16)Data;
				break;

			case PAGESIZE_WORD:
				(*(volatile u32*)MemoryAddress) = (u32)Data;
				break;

			case PAGESIZE_DOUBLE_WORD:
				(*(volatile u32*)MemoryAddress) = (u64)Data;
				break;

			}


			while ((FLASH->SR >> SR_BUSY_BIT) == 1) {
				/*Flashing on progress*/
			}/*end of while*/
		}

		LOCK_FLASH_ACTION;

	}

	return Loc_enuErrorStatus;

}
