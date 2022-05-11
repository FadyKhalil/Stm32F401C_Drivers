/*
 * Fprog.h
 *
 *  Created on: May 10, 2022
 *      Author: fady
 */

#ifndef MCAL_FPROG_FPROG_H_
#define MCAL_FPROG_FPROG_H_

typedef enum
{
	Fpro_enuOK = 0,
	Fprog_enuNullPointer,
	Fprog_enuControlRegisterLock,
	Fprog_enuSectorNumberWrong,
	Fprog_enuExceedFlash


}Fprog_tenuErrorStatus;


typedef struct
{
	u8 Error;
	u8 EndOfOperation;

}Fprog_Interrupt;

typedef struct
{
	u8 FlashAccessLatency;
	Fprog_Interrupt Interrupt;

}Fprog_Config;

/********************DEFINES*********************/

/*this Option relevant the the structure Fprog_Config
 *
 * !Note: Using this bit depends on the voltage level connected
 *
 * */
#define PAGESIZE_BYTE			0x00
#define PAGESIZE_HALF_WORD		0x01
#define PAGESIZE_WORD			0x02
#define PAGESIZE_DOUBLE_WORD	0x03

/*SectorNumber
 *
 * !NOTE : STM32F401C
 *
 * */
#define SECTOR_0				0
#define SECTOR_1				1
#define SECTOR_2				2
#define SECTOR_3				3
#define SECTOR_4				4
#define SECTOR_5				5


/*
 * Function Name:				Fprog_vidInit
 *
 * Description:					Configure the Flash driver
 *
 * parameter:					ConfigPtr (pointer to structure)
 *
 * return:						void
 * */
extern void Fprog_vidInit(const Fprog_Config* ConfigPtr);

/*
 * Function Name:				Fprog_enuMassErase
 *
 * Description:					To delete all Flash memory
 *
 * parameter:					void
 *
 * return:						void
 * */
extern Fprog_tenuErrorStatus Fprog_enuMassErase(void);

/*
 * Function Name:				Fprog_enuSectorErase
 *
 * Description:					Sector Number to be deleted
 *
 * parameter:					SectorNumber (Sector Number to be deleted)
 * 										in range {SECTOR_1, SECTOR_2, SECTOR_3, SECTOR_4, SECTOR_5}
 *
 * return:						ErrorStatus
 * */
extern Fprog_tenuErrorStatus Fprog_enuSectorErase(u8 SectorNumber);

/*
 * Function Name:				Fprog_enuOneTimeProgramming
 *
 * Description:					This is a section in a memory can be only programed one time
 * 											& never can be erased in run time to rewrite on it you need to powerOFF/PowerON
 * 											the Microcontroller
 *
 * parameter:					Data --> Address of the variable or Buffer
 * 								BufferSize --> size of the buffer
 *
 * 								if you will send a normal variable with one index then SET BufferSize with 1
 *
 * return:						ErrorStatus
 *
 * !!!! NOTE = Make sure the Buffersize is multiple of 2 always and the data sent MUST be at LEAST 8 BYTE * n;
 * 			MIN n = 1 , MAX n = 16;
 * */
extern Fprog_tenuErrorStatus Fprog_enuOneTimeProgramming(pu32 Data, u8 BufferSize);


/*
 * Function Name:				Fprog_enuProgrammingFlash
 *
 * Description:					Program Flash memory
 *
 * parameter:					@ Data --> Address of the variable or Buffer
 * 								@ MemoryAddress --> the flash Adress to store the variable
 * 									in range {0x08000000 ->  0x0803FFFF}
 *								@ Copy_u8Pagesize --> Pagesize to program the data
 *									in range {PAGESIZE_BYTE, PAGESIZE_HALF_WORD, PAGESIZE_WORD, PAGESIZE_DOUBLE_WORD}
 *
 * return:						ErrorStatus
 * */
extern Fprog_tenuErrorStatus Fprog_enuProgrammingFlash(u64 Data, u32 MemoryAddress, u8 Copy_u8Pagesize);








#endif /* MCAL_FPROG_FPROG_H_ */
