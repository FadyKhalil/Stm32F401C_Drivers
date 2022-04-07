/*
 * Lcd_cfg.c
 *
 *  Created on: Mar 15, 2022
 *      Author: fady
 */
#include "Std_types.h"
#include "Gpio.h"
#include "Lcd_prv.h"
#include "Lcd_cfg.h"
#include "Lcd.h"


Lcd_tstrPinConfiguration Lcd_strPin[LCD_NUMBER_PIN] = {


		[LCD_u8RS_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN00,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8RW_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN01,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8EN_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN02,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D0_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN03,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D1_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN04,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D2_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN05,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D3_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN06,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D4_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN07,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D7_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN08,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D6_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN09,
			.Speed = LCD_LOW_SPEED
		},

		[LCD_u8D5_PIN] =
		{
			.Port = PORTA,
			.Pin = GPIO_PIN10,
			.Speed = LCD_LOW_SPEED
		},
};

void Lcd_EnableDelay(void)
{
//	Dio_enuWriteChannel(LCD_u8EN_PIN, DIO_u8HIGH);
//	_delay_ms(2);
//	Dio_enuWriteChannel(LCD_u8EN_PIN, DIO_u8LOW);
}
