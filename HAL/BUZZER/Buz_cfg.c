/*
 * lec_cfg.c
 *
 *  Created on: Mar 27, 2022
 *      Author: fady
 */

#include "Buz_cfg.h"

#include "../../MCAL/GPIO/Gpio.h"
#include "Buz.h"

const Buz_tstrConfiguration Buz_astrleds[BUZ_NUMBER] =
{
		[BUZ_ALARM] =
		{
			.Pin = GPIO_PIN04,
			.Active_states = BUZ_CLOSE,
			.Port = PORTA,
			.Otype = BUZ_OTYPE_PP,
			.Speed = BUZ_LOW_SPEED
		},
};
