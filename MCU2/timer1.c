/*
 * timer1.c
 *
 *  Created on: Nov 1, 2022
 *      Author: georg
 */

#include <avr/interrupt.h>
#include "timer1.h"
#include <avr/io.h>

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCCR1A = (1<<FOC1A);							// not PWM mode
	TCCR1B |= (Config_Ptr->prescaler<<CS10) | (Config_Ptr->mode<<WGM12);
	TCNT1 = Config_Ptr->initial_value;										// start point = 0
	if(Config_Ptr->mode==1)
	{
		OCR1A = Config_Ptr->compare_value;									// catch point
	}
	TIMSK |= (1<<OCIE1A);							// enable interrupt
}

void Timer1_deInit(void)
{
	TCCR1B &= ~(0x07);
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
