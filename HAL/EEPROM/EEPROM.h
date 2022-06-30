/* **************************************************************************************
**       Author      :	Smart Safe Group
**       Date        :	June 1, 2022
**       Version     :	V01
**       SWC         :	EEPROM
**       Description :	EEPROM module files
** **************************************************************************************/
#ifndef HAL_EEPROM_EEPROM_H_
#define HAL_EEPROM_EEPROM_H_
  extern void EEPROM_WriteByte(u16 Copy_u16Address, u8 Copy_u8Data);
  extern void EEPROM_ReadByte(u16 Copy_u16Address, u8* Add_pu8Data);
#endif /* HAL_EEPROM_EEPROM_H_ */
