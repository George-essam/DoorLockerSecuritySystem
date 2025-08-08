/*
 * CONTROL_ECU.c
 *
 *  Created on: Nov 1, 2022
 *      Author: georg
 */

#include "Buzzer.h"
#include "external_eeprom.h"
#include "timer1.h"
#include "uart.h"
#include "util/delay.h"
#include "dc_motor.h"
#include <avr/io.h>

#define Adr 			0x0311
#define Wrong_Pass 		12
#define Correct_Pass 	34
#define READY 			40
#define CW				1
#define ACW 			2
#define OFF 			0
#define Door_Open 		10
#define _3sec 			3000
#define _15sec 			15000
#define Buzzer_ON 		22
#define Change_Pass		44


uint8 pass1[5], pass2[5], pass3[5], pass4[5], gtick=0;



void Timer_Callback(void)
{
	gtick++;		/* global tick that increment every second*/
}


void ECU_init(void)
{
	SREG |= (1<<7);
	UART_ConfigType Config_UART = {Eight_bitdata,Disabled,one_stopbit,BR_9600};
	/*config of UART as eight bit frame , no parity, one stop bit and 9600 baudrate*/
	TWI_ConfigType Config_TWI = {adress,Normal_mode};
	/*config of TWI as 01 adress, normal mode*/
	Timer1_ConfigType Config_Timer1 = {0,8000,Pre_1024,CTC};
	/*config of Timer1 as zero start point , 8000 catch point (1 sec), 1024 pre scale
	 and on catch mood*/
	UART_init(&Config_UART);
	TWI_init(&Config_TWI);
	Timer1_init(&Config_Timer1);
	Timer1_setCallBack(Timer_Callback);
	DcMotor_Init();
	Buzzer_Init();
}

void Recieve_Pass(void)
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		while(!UART_recieveByte());
		pass1[i] = UART_recieveByte();		/*save first pass in pass1 array*/
	}
	for(i=0;i<5;i++)
	{
		while(!UART_recieveByte());
		pass2[i] = UART_recieveByte();		/*save the second pass in pass2 array*/
	}
}


uint8 Compare_Pass(void)
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		if(pass1[i] != pass2[i])	/*compare the first with the second pass*/
		{
			return 0;				/*return 0 if password is not correct*/
		}
	}
	return 1;						/*return 1 if password is correct*/
}


void Save_Pass(void)
{
	unsigned char i;
	for(i=0;i<5;i++)						/*for loop to save the 5 numbers*/
	{
		EEPROM_writeByte(Adr+i, pass1[i]);	/*save password in memory*/
		_delay_ms(10);
	}
}


uint8 Check_Pass_Mem(void)
{
	unsigned char i,val;
	for(i=0;i<5;i++)
	{
		while(!UART_recieveByte());
		pass3[i] = UART_recieveByte();		/*save the third pass in pass3 array*/
	}

	for(i=0;i<5;i++)
	{
		EEPROM_readByte(Adr+i, &val);		/*fetch pass1 that already saved in memory*/
		if(pass3[i] != val)					/*compare pass3 with the fetched password*/
		{
			return 0;						/*return 0 if password is not correct*/
		}
		_delay_ms(10);
	}
	return 1;								/*return 1 if password is correct*/
}


void Open_Door(void)
{
	gtick=0;					/* set gtick to zero to start counting*/
	DcMotor_Rotate(CW,99);		/*activate the motor in clock wise*/
	while(gtick<15);			/*counting 15 second*/
	DcMotor_Rotate(OFF,0);		/*deactivate the motor*/
	gtick=0;					/* set gtick to zero to start counting*/
	while(gtick<3);
	DcMotor_Rotate(ACW,99);		/*activate the motor in Anti clock wise*/
	gtick=0;					/* set gtick to zero to start counting*/
	while(gtick<15);			/*counting 15 second*/
	DcMotor_Rotate(OFF,0);		/*deactivate the motor*/
	Timer1_deInit();			/*deactivate the timer*/
}



int main(void)
{
	unsigned char state,flag=0,i=0;
	ECU_init();
	while(1)
	{
		while(flag==0)					/*flag set when pass is correct*/
		{
			Recieve_Pass();
			if(Compare_Pass())			/*step0 compare*/
			{
				Save_Pass();
				_delay_ms(500);
				UART_sendByte(READY);
				UART_sendByte(Correct_Pass);
				flag=1;
			}
			else
			{
				_delay_ms(500);
				UART_sendByte(READY);			/*send ready bit to interface*/
				UART_sendByte(Wrong_Pass);
			}
		}
		while(flag==1 && i<3)
		{
			if(Check_Pass_Mem())
			{
				_delay_ms(500);
				UART_sendByte(READY);			/*send ready bit to interface*/
				UART_sendByte(Correct_Pass);
				flag=0;
			}
			else
			{
				_delay_ms(500);
				UART_sendByte(READY);			/*send ready bit to interface*/
				UART_sendByte(Wrong_Pass);
				i++;
			}
		}
		while(!UART_recieveByte());
		state = UART_recieveByte();
		if(state==Door_Open)					/*open the door*/
		{
			Open_Door();
		}
		else if(state == Change_Pass)			/*change the password*/
		{
			_delay_ms(500);
		}
		else if(state == Buzzer_ON)				/*activate the buzzer*/
		{
			gtick=0;
			Buzzer_On();
			while(gtick<60);					/*wait for 60 second*/
			Buzzer_Off();						/*Deactivate the buzzer*/
		}
	}
}
