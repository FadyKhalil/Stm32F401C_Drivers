/* **************************************************************************************
**       Author      :	Smart Safe Group
**       Date        :	June 1, 2022
**       Version     :	V01
**       SWC         :	I2C
**       Description :	I2C module files
** **************************************************************************************/
#include "StdTypes.h"
#include "I2C.h"
typedef struct
{
	u32 CR1;
	u32 CR2;
	u32 OAR1;
	u32 OAR2;
	u32 DR;
	u32 SR1;
	u32 SR2;
	u32 CCR;
	u32 TRISE;
	u32 FLTR;
}I2C_Reg;
//I2C1 (PB6/PB7)
/**************DEFINE**************/
#define I2C(PROTOCOL)		((volatile I2C_Reg*)(PROTOCOL))
/*Accessing bit in CR1 Register*/
#define ACK						10
#define POS						11
#define CLOCK_BIT_STRETCHING	7
#define PREPHERAL_BIT_ENABLE	0
#define START_BIT				8
#define STOP_BIT				9
#define BTF_BIT					2
#define ADDR_BIT				1
/*Accessing bit in CR2 Register*/
#define ITERREN					8
#define ITEVTEN					9
#define FREQ					  0
/*Accessing bit in CRR Register*/
#define MODE_SLC				15
#define DUTY_CYCLE			14
/*Accessing bit in CRR Register*/
#define TXE_BIT					7
#define RXE_BIT					6
/*Initializing*/
#define PREPHERAL_ENABLE		1
/*Private Global Variable*/
//static u8 I2C_Data_Send = 0;
I2C_enuErrorStatus I2C_enuVidInit(const I2C_Config* ConfigPtr)
{
	I2C_enuErrorStatus Loc_enuErrorStatus = I2C_enuOk;
	u16 Loc_u16TempRegister = 0;
	if(ConfigPtr)
	{
		/*Reset I2C Prepheral*/
		I2C(ConfigPtr->Protocol)->CR1 |= (1 << 15);
		/*Take it out from reset Mode*/
		I2C(ConfigPtr->Protocol)->CR1 &= ~(1 << 15);
		/*Initializing CR2 Register
		 * --> Error Interrupt
		 * --> Event Interrupt
		 * --> Clock Frequency
		 *
		 * !! Loc_u16TempRegister to accessing the register once
		 *
		 * */
		/*Configure CR1 Register*/
		Loc_u16TempRegister = I2C(ConfigPtr->Protocol)->CR2;
		Loc_u16TempRegister &= ~(0x003F);
		Loc_u16TempRegister = ConfigPtr->Error_Int_State << ITERREN
							| ConfigPtr->Event_Int_State << ITEVTEN
							| ConfigPtr->Clock_Selection <<	FREQ;
		/*Assign value to the Protocol Control register*/
		I2C(ConfigPtr->Protocol)->CR2 = Loc_u16TempRegister;
		Loc_u16TempRegister = 0;
		I2C(ConfigPtr->Protocol)->CCR |= ConfigPtr->Mode_Selection << MODE_SLC;
		I2C(ConfigPtr->Protocol)->CCR |= ConfigPtr->Duty_Cycle;
		if(ConfigPtr->Mode_Selection == STANDAR_MODE)
		{
			/*
			 * 16000000 it should depend of the freq chosen in CR2 but we will pretend is 16Mgz
			 * for this version
			 *
			 * */
			Loc_u16TempRegister = ((u32)16000000/ConfigPtr->Desired_Duty_Cycle)/2;
		}
		I2C(ConfigPtr->Protocol)->CCR |= Loc_u16TempRegister;
		/*	the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125
		 * 	therefore the TRISE[5:0] bits must be programmed with 9.
		 *  (1000 ns / 62.5 ns = 16 + 1)
		 */
		I2C(ConfigPtr->Protocol)->TRISE = 17;
		/* Clock Streching Initialize*/
		I2C(ConfigPtr->Protocol)->CR1 |= ConfigPtr->Clock_Stretching << CLOCK_BIT_STRETCHING;
		/*Prepheral Enable*/
		I2C(ConfigPtr->Protocol)->CR1 |= PREPHERAL_ENABLE << PREPHERAL_BIT_ENABLE;
	}
	else
	{
		Loc_enuErrorStatus = I2C_enuNullPointer;
	}
	return Loc_enuErrorStatus;
}
void I2C_enuSendStart(void* Protocol)
{
	if(Protocol)
	{
		/*Start Condition*/
		I2C(Protocol)->CR1 |= ENABLE << START_BIT;
		/*Wait till SB is set*/
		while(((I2C(Protocol)->SR1) & 1) == 0);
	}
	else
	{
		/*Do nothing*/
	}
}
void I2C_enuSendStop(void* Protocol)
{
	if(Protocol)
	{
		/*Stop Condition*/
		I2C(Protocol)->CR1 |= ENABLE << STOP_BIT;
	}
}
I2C_enuErrorStatus I2C_enuSendAddressSynchronous(void* Protocol, I2C_Slave_Add Copy_u16Adress, u8 Copy_u8ReadOrWrite)
{
	I2C_enuErrorStatus Loc_enuErrorStatus = I2C_enuOk;
	/*Check if its NULL*/
	if(Protocol)
	{
		u8 Loc_u8TempRegister  = 0;
		/*Set the LSB for writing mode */
		Loc_u8TempRegister = (Copy_u16Adress | Copy_u8ReadOrWrite);
		/*Reading SR1 Register*/
		(void)I2C(Protocol)->SR1;
		/*Write the Address*/
		I2C(Protocol)->DR = Loc_u8TempRegister;
		/*Wait BTF to be set to check the data is sent*/
		while(((I2C(Protocol)->SR1) & (1 << ADDR_BIT)) == 0);
		/*Clear the Register for next step*/
		(void)(I2C(Protocol)->SR1|I2C(Protocol)->SR2);
	}
	else
	{
		Loc_enuErrorStatus = I2C_enuNullPointer;
	}
	/*Return the error Status if any*/
	return Loc_enuErrorStatus;
}
I2C_enuErrorStatus I2C_enuSendByteSynchronous(void* Protocol, u8 Copy_u8Data)
{
	I2C_enuErrorStatus Loc_enuErrorStatus = I2C_enuOk;
	if(Protocol)
	{
		/*Check the transmitter flag
		 * Check the TXE flag
		 * */
		while(((I2C(Protocol)->SR1 >> TXE_BIT) & 1) == 0);
		/*Write the Data*/
		I2C(Protocol)->DR = Copy_u8Data;
		/*Check if the Data transmit by checking BTF flag*/
		while(((I2C(Protocol)->SR1 >> BTF_BIT) & 1) == 0);
	}
	else
	{
		Loc_enuErrorStatus = I2C_enuNullPointer;
	}
	return Loc_enuErrorStatus;
}
I2C_enuErrorStatus I2C_enuReceiveByteSynchronous(void* Protocol, pu8 Add_pu8Data)
{
	I2C_enuErrorStatus Loc_enuErrorStatus = I2C_enuOk;
	if(Protocol)
	{
		/*Wait the data to be transfer
		 * Check The RXE flag in SR1 register
		 * */
		while(((I2C(Protocol)->SR1 >> RXE_BIT) & 1) == 0);
		/*Receive The data*/
		*Add_pu8Data = I2C(Protocol)->DR;
		/*Send Acknowledge*/
		I2C(Protocol)->CR1 |= 1 << ACK;
	}
	else
	{
		Loc_enuErrorStatus = I2C_enuNullPointer;
	}
	return Loc_enuErrorStatus;
}