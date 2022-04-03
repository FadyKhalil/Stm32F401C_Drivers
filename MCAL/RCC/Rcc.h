/*
 * Rcc.h
 *
 *  Created on: Mar 12, 2022
 *      Author: fady
 */

#ifndef RCC_H_
#define RCC_H_


/*------------------------------------------TYPES------------------------------------------*/

typedef enum
{
	Rcc_OK = 5,
	Rcc_InputError,
	Rcc_ClockNotReady,
	Rcc_PllEnable,
	Rcc_InputRangeError
}Rcc_tenuErrorStatus;

typedef enum
{
	Rcc_HsiClockSytem = 0,
	Rcc_HseClockSytem,
	Rcc_PllClockSytem

}Rcc_tenuClockSystem;

/*-----------------------------------------DEFINE-------------------------------------------*/

/*Parameter to be used for function :
 * @	Rcc_SetClockON
 * @	Rcc_ReadStatusClock
 * */
#define HSI_CLOCK	0x10000001
#define HSE_CLOCK	0x10010000
#define PLL_CLOCK	0x11000000

/*Parameter to be used for function:
 * @	Rcc_SetPllClock
 * */
#define PLL_HSI		0x20000000
#define PLL_HSE		0x20400000

/*Parameter to be used for function:
 * @	Rcc_SetClockSystem
 * */
#define HSI_CLOCK_SYSTEM	0x30000000
#define HSE_CLOCK_SYSTEM	0x30000001
#define PLL_CLOCK_SYSTEM	0x30000002

/*Parameter to be used for function:
 * @	Rcc_ReadClockSystemStatus
 * */
#define HSI_CLOCK_SYSTEM_STATUS		0x00000000
#define HSE_CLOCK_SYSTEM_STATUS		0x00000004
#define PLL_CLOCK_SYSTEM_STATUS		0x00000008

/*Parameter to be used for function:
 * @	Rcc_ResetAHB1Peripheral
 * @	Rcc_SetAHB1Peripheral
 * */
#define SET_RESET_GPIOA			0x40000001
#define SET_RESET_GPIOB			0x40000002
#define SET_RESET_GPIOC			0x40000004
#define SET_RESET_GPIOD			0x40000008
#define SET_RESET_GPIOE			0x40000010
#define SET_RESET_GPIOH			0x40000080

/*Parameter to be used for function:
 * @	Rcc_ResetAPB2Peripheral
 * @	Rcc_SetAPB2Peripheral
 * */
#define SET_RESET_TIM1			0x50000001
#define SET_RESET_USART1		0x50000010
#define SET_RESET_USART6		0x50000020
#define SET_RESET_ADC1			0x50000100
#define SET_RESET_SDIO			0x50000800
#define SET_RESET_SPI1			0x50001000
#define SET_RESET_SPI4			0x50002000
#define SET_RESET_SYSTEMCFGR	0x50004000
#define SET_RESET_TIM9			0x50100000
#define SET_RESET_TIM10			0x50200000
#define SET_RESET_TIM11			0x50400000

/*Parameter to be used for function:
 * @	Rcc_ResetAPB1Peripheral
 * @	Rcc_SetAPB1Peripheral
 * */
#define SET_RESET_TIM2			0x60000001
#define SET_RESET_TIM3			0x60000002
#define SET_RESET_TIM4			0x60000004
#define SET_RESET_TIM5			0x60000008
#define SET_RESET_WWDG			0x60000800
#define SET_RESET_SPI2			0x60004000
#define SET_RESET_SPI3			0x60008000
#define SET_RESET_USART2		0x60020000
#define SET_RESET_I2C1			0x60200000
#define SET_RESET_I2C2			0x60400000
#define SET_RESET_I2C3			0x60800000




/*------------------------------------FUNCTION PROTOTYPE------------------------------------*/

/*
 * Public Function:		Rcc_SetClockOn
 * Input Parameter:		Copy_u8ClockSelection in range {HSI_CLOCK, HSEON_CLOCK, PLLON_CLOCK}
 *
 * Description:			This function to set the clock system
 *
 * Return:				Error Input if it's not from the range
 *
 * */
Rcc_tenuErrorStatus Rcc_SetClockOn(u32 Copy_u32ClockSelection);


/*
 * Public Function:		Rcc_ReadStatusClock
 * Input Parameter:		Copy_u8ClockSelection in range {HSI_CLOCK, HSEON_CLOCK, PLLON_CLOCK}
 *
 * Description:			This function is to read the status of the clock set ready or not
 *
 * Return:				-> If the clock fail to be ready will return Error Status
 *						-> if the input not in range
 * */
Rcc_tenuErrorStatus	Rcc_ReadStatusClock(u32 Copy_u32ClockSelection);


/*
 * Public Function  		:	Rcc_SetHseClockBypass
 * Description				:	This Function is to set the bypass clock to bypass the oscillator with an external clock
 *
 *!!!Constrain				:	You can only set it if and only if the you didn't set the clock system
 *
 * Input Parameter			:	Not-Applicable	(void)
 *
 * Return					:	Not-Applicable (void)
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
void Rcc_SetHseClockBypass(void);


/*
 * Public Function  		:	Rcc_SetPllClock
 * Description				:	This Function is to set the PLL clock
 *
 *!!!Constrain				:	you need to set the clock before configure the PLL
 *
 * Input Parameter:			:	Copy_u8ClockSelection in range {PLL_HSI, PLL_HSE}
 *
 * Return					:	-> Wrong Input
 * 								-> The PLL in ON
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus Rcc_SetPllClock(u32 Copy_u32ClockSelection);


/*
 * Public Function  		:	Rcc_PllConfigureation
 * Description				:	This Function is to set the bypass clock to bypass the oscillator with an external clock
 *
 *!!!Constrain				:	You can only configure the PLL if the PLL is disable
 *
 *Input Parameter			:	@ Copy_u8PllM in range (2 -> 63)
 * 								@ Copy_u8PllN in range (192 -> 432)
 * 								@ Copy_u8PllQ in range (2 -> 15)
 * 								@ Copy_u8PllP in range that the output clock does not exceed 84 MHZ
 *
 *!!Attention				:	The default clock is HSI in case you didn't set PLL Clock
 *
 * Return					:	Input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_PllConfiguration(u8 Copy_u8PllM, u16 Copy_u16PllN, u8 Copy_u8PllQ, u8 Copy_u8PllP);


/*
 * Public Function			:	Rcc_SetClockSystem
 *
 * Description				:	This function to set the clock system that will feed the processor
 * 								& peripheral...
 *
 * Input Parameter			:	Copy_u8ClockSlection in range (HSI_CLOCK_SYSTEM, HSE_CLOCK_SYSTEM, PLL_CLOCK_SYSTEM)
 *
 * Return 					: 	Error Status if the input out of range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_SetClockSystem (u32 Copy_u32ClockSlection);



/*
 * Public Function:		Rcc_ReadSystemClockStatus
 * Input Parameter:		Copy_u8ClockSelection in range {HSI_CLOCK, HSEON_CLOCK, PLLON_CLOCK}
 *
 * Description:			This function is to read the status of the system clock
 *
 * Return:				-> the clock of the system
 *
 * */
Rcc_tenuClockSystem Rcc_ReadSystemClockStatus (void);


/*
 * Public Function			:	Rcc_ResetAHB1Peripheral
 *
 * Description				:	This function is to reset the AHB1 peripheral (it disable the clock)
 *
 * Input Parameter			:	Copy_u8AHB1Peripheral in range
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_ResetAHB1Peripheral(u32 Copy_u32AHB1Peripheral);


/*
 * Public Function			:	Rcc_ResetAPB1Peripheral
 *
 * Description				:	This function is to reset the ApB1 peripheral (it disable the clock)
 *
 * Input Parameter			:	u8 Copy_u8APB2Peripheral in range {}
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_ResetAPB1Peripheral(u32 Copy_u32APB1Peripheral);


/*
 * Public Function			:	Rcc_ResetAPB2Peripheral
 *
 * Description				:	This function is to reset the APB2 peripheral (it disable the clock)
 *
 * Input Parameter			:	u8 Copy_u8APB2Peripheral in range {}
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_ResetAPB2Peripheral(u32 Copy_u32APB2Peripheral);


/*
 * Public Function			:	Rcc_SetAHB1Peripheral
 *
 * Description				:	This function is to reset the AHB1 peripheral (it disable the clock)
 *
 * Input Parameter			:	Copy_u8AHB1Peripheral in range
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *								@ SET_RESET_GPIO
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_SetAHB1Peripheral(u32 Copy_u32AHB1Peripheral);


/*
 * Public Function			:	Rcc_SetAPB1Peripheral
 *
 * Description				:	This function is to reset the ApB1 peripheral (it disable the clock)
 *
 * Input Parameter			:	u8 Copy_u8APB2Peripheral in range
 *
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_SetAPB1Peripheral(u32 Copy_u32APB1Peripheral);


/*
 * Public Function			:	Rcc_SetAPB2Peripheral
 *
 * Description				:	This function is to reset the APB2 peripheral (it disable the clock)
 *
 * Input Parameter			:	u8 Copy_u8APB2Peripheral in range {}
 *
 * Return 					: 	Error Status if the input not in range
 *
 * Input/Output Parameter	:	Not-Applicable
 *
 * */
Rcc_tenuErrorStatus	Rcc_SetAPB2Peripheral(u32 Copy_u32APB2Peripheral);



#endif /* RCC_H_ */
