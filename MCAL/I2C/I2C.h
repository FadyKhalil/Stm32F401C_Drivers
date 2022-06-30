/* **************************************************************************************
**       Author      :	Smart Safe Group
**       Date        :	June 1, 2022
**       Version     :	V01
**       SWC         :	I2C
**       Description :	I2C module files
** **************************************************************************************/
#ifndef MCAL_I2C_I2C_H_
#define MCAL_I2C_I2C_H_
  typedef enum
  {
    I2C_enuOk = 0,
    I2C_enuNOk,
    I2C_enuNullPointer
  }I2C_enuErrorStatus;
  typedef struct
  {
    u8 Clock_Selection;
    u8 Data_Reception;
    u8 Acknowledge_State;
    u8 Error_Int_State;
    u8 Event_Int_State;
    u8 Mode_Selection;
    u8 Duty_Cycle;
    u8 Clock_Stretching;
    u32 Desired_Duty_Cycle;
    void* Protocol;
  }I2C_Config;
  typedef u16 I2C_Slave_Add;
  /************Define use for configuration*****************/
  /*Protocal Slection*/
  #define I2C_1 		(u32*)0x40005400
  #define I2C_2 		(u32*)0x40005800
  #define I2C_3 		(u32*)0x40005C00
  /*Mode Selection*/
  #define FAST_MODE			1
  #define STANDAR_MODE		0
  /*State*/
  #define ENABLE				1
  #define DISABLE				0
  /*Clock Stretching*/
  #define CLOCK_STRETCHING_ENABLE		0
  #define CLOCK_STRETCHING_DISABLE	1
  /*RW accessing*/
  #define I2C_WRITE					0
  #define I2C_READ					1
  /******************Prototype Of Function*****************/
  I2C_enuErrorStatus I2C_enuVidInit(const I2C_Config* ConfigPtr);
  void I2C_enuSendStart(void* Protocol);
  void I2C_enuSendStop(void* Protocol);
  I2C_enuErrorStatus I2C_enuSendAddressSynchronous(void* Protocol, I2C_Slave_Add Copy_u16Adress, u8 Copy_u8ReadOrWrite);
  I2C_enuErrorStatus I2C_enuSendByteSynchronous(void* Protocol, u8 Copy_u8Data);
  I2C_enuErrorStatus I2C_enuReceiveByteSynchronous(void* Protocol, pu8 Add_pu8Data);
#endif /* MCAL_I2C_I2C_H_ */
