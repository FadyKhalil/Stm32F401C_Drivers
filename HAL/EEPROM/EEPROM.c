/* **************************************************************************************
**       Author      :	Smart Safe Group
**       Date        :	June 1, 2022
**       Version     :	V01
**       SWC         :	EEPROM
**       Description :	EEPROM module files
** **************************************************************************************/
#include "StdTypes.h"
#include "I2C.h"
#include "EEPROM.h"

#define EEPROM_ADDRESS		0xA0

void EEPROM_WriteByte(u16 Copy_u16Address, u8 Copy_u8Data)
{
	I2C_enuSendStart(I2C_1);

	I2C_enuSendAddressSynchronous(I2C_1, EEPROM_ADDRESS, I2C_WRITE);

	I2C_enuSendByteSynchronous(I2C_1, (u8)Copy_u16Address);

	I2C_enuSendByteSynchronous(I2C_1, Copy_u8Data);

	I2C_enuSendStop(I2C_1);

	OS_vidDelay(5);
}


void EEPROM_ReadByte(u16 Copy_u16Address, u8* Add_pu8Data)
{
	I2C_enuSendStart(I2C_1);

	I2C_enuSendAddressSynchronous(I2C_1, EEPROM_ADDRESS, I2C_WRITE);

	I2C_enuSendByteSynchronous(I2C_1, (u8)Copy_u16Address);
	OS_vidDelay(5);
//	for(volatile int i = 0; i < 1000; i++)
//	{
//
//	}

	I2C_enuSendStart(I2C_1);

	I2C_enuSendAddressSynchronous(I2C_1, EEPROM_ADDRESS, I2C_READ);

	I2C_enuReceiveByteSynchronous(I2C_1, Add_pu8Data);

	I2C_enuSendStop(I2C_1);

}

