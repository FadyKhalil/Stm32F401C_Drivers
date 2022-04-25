/*
 * Switch_cfg.c
 *
 *  Created on: Apr 4, 2022
 *      Author: fady
 */

#include "../../MCAL/GPIO/Gpio.h"
#include "Switch.h"
#include "Switch_cfg.h"

Switch_tstrConfiguration Switch_astrSwitchs[NUMBER_OF_SWITCH] = {
		[SWITCH_IGNITION] = {
				.Mode = SWITCH_PULLUP,
				.Port = PORTA,
				.Pin = GPIO_PIN01
		},

		[SWITCH_DOOR] = {
				.Mode = SWITCH_PULLUP,
				.Port = PORTA,
				.Pin = GPIO_PIN00
		},


};

