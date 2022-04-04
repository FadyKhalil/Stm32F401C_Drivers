/*
 * Switch_cfg.c
 *
 *  Created on: Apr 4, 2022
 *      Author: fady
 */


#include "Gpio.h"
#include "Switch.h"
#include "Switch_cfg.h"

Switch_tstrConfiguration Switch_astrSwitchs[NUMBER_OF_SWITCH] = {
		[SWITCH_00] = {
				.Mode = SWITCH_PULLUP,
				.Port = PORTA,
				.Pin = GPIO_PIN01
		},

		[SWITCH_01] = {
				.Mode = SWITCH_PULLUP,
				.Port = PORTA,
				.Pin = GPIO_PIN02
		},

		[SWITCH_02] = {
				.Mode = SWITCH_PULLUP,
				.Port = PORTA,
				.Pin = GPIO_PIN03
		}
};

