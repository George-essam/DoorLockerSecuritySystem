/*
 * dc_motor.c
 *
 *  Created on: Oct 8, 2022
 *      Author: georg
 */

#include"dc_motor.h"
#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* For GET_BIT Macro */
#include "gpio.h"
#include"pwm.h"

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(motorout1_portID,motorout1_pinID,PIN_OUTPUT);
	GPIO_setupPinDirection(motorout2_portID,motorout2_pinID,PIN_OUTPUT);
	/* Motor is stop at the beginning */
	GPIO_writePin(motorout1_portID,motorout1_pinID,LOGIC_LOW);
	GPIO_writePin(motorout2_portID,motorout2_pinID,LOGIC_LOW);

}

void DcMotor_Rotate(uint8 state,uint8 speed)
{
	if(state==2)
	{
		GPIO_writePin(motorout1_portID,motorout1_pinID,LOGIC_HIGH);
		GPIO_writePin(motorout2_portID,motorout2_pinID,LOGIC_LOW);
	}
	if(state==1)
	{
		GPIO_writePin(motorout1_portID,motorout1_pinID,LOGIC_LOW);
		GPIO_writePin(motorout2_portID,motorout2_pinID,LOGIC_HIGH);

	}
	else if(state==0)
	{
		GPIO_writePin(motorout1_portID,motorout1_pinID,LOGIC_LOW);
		GPIO_writePin(motorout2_portID,motorout2_pinID,LOGIC_LOW);
	}
	PWM_Timer0_Start(speed);
}
