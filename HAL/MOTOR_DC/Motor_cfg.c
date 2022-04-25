/*
 * Motor_cfg.c
 *
 *  Created on: Apr 5, 2022
 *      Author: fady
 */


#include "../../MCAL/GPIO/Gpio.h"
#include "Motor.h"
#include "Motor_cfg.h"


Motor_tstrConfiguration Motor_strMotors[NUMBER_OF_MOTOR] = {

		[MOTOR_ENGINE] = {

				.Pin = GPIO_PIN03,
				.Otype = MOTOR_OTYPE_PP,
				.Port = PORTA,
				.Speed = MOTOR_LOW_SPEED,
				.Active_states = MOTOR_CLOSE

		}

};

