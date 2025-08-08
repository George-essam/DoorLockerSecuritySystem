/*
 * Buzzer.c
 *
 *  Created on: Oct 31, 2022
 *      Author: georg
 */

#include "Buzzer.h"
#include "avr/io.h"
#include "common_macros.h"
#include "gpio.h"



void Buzzer_Init(void)
{
	GPIO_setupPinDirection(BuzzerPort,BuzzerPin,PIN_OUTPUT);
	GPIO_writePin(BuzzerPort,BuzzerPin,LOGIC_LOW);
}

void Buzzer_On(void)
{
	GPIO_writePin(BuzzerPort,BuzzerPin,LOGIC_HIGH);
}

void Buzzer_Off(void)
{
	GPIO_writePin(BuzzerPort,BuzzerPin,LOGIC_LOW);
}
