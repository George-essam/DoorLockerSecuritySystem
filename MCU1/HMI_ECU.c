
/******************************************************************************
 *
 * Module: Human interface ECU
 *
 * File Name: HMI_ECU.c
 *
 * Description: Main file of interfacing
 *
 * Author: George Essam
 *
 *******************************************************************************/


#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <avr/io.h>
#include "util/delay.h"
#include "std_types.h"



#define READY 			40
#define Wrong_Pass 		12
#define Correct_Pass 	34
#define Door_Open 		10
#define _3sec 			3000
#define _15sec 			15000
#define Buzzer_ON 		22
#define STEP0 			44


uint8 gtick=0;
unsigned char step=0,key,i;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void Timer_Callback(void)
{
	gtick++;
}


void ECU_init(void)
{
	SREG |= (1<<7);
	UART_ConfigType Config_UART = {Eight_bitdata,Disabled,one_stopbit,BR_9600};
	/*config of UART as eight bit frame , no parity, one stop bit and 9600 baudrate*/
	Timer1_ConfigType Config_Timer1 = {0,8000,Pre_1024,CTC};
	UART_init(&Config_UART);
	LCD_init();
	Timer1_init(&Config_Timer1);
	Timer1_setCallBack(Timer_Callback);
}


void Get_Send_Pass(void)
{
	key = KEYPAD_getPressedKey();			/*get number from the keypad*/
	if((key <= 9) && (key >= 0) && i<5)		/*get only 5 numbers as password*/
	{
		LCD_displayCharacter('*');			/*display '*' as number*/
		UART_sendByte(READY);				/*send ready bit to controller*/
		UART_sendByte(key);					/*send number to be saved or compared*/
		i++;
	}
}


void Buzz(void)
{
	UART_sendByte(READY);						/*send ready bit to controller*/
	UART_sendByte(Buzzer_ON);					/*activate buzzer alert in controller*/
	LCD_displayStringRowColumn(0,0,"ERROR");
	gtick=0;
	while(gtick<60);
	step=2;
	LCD_clearScreen();
}


void Door(void)
{
	UART_sendByte(READY);							/*send ready bit to controller*/
	UART_sendByte(Door_Open);						/*open and close the door in controller*/
	LCD_displayStringRowColumn(0,0,"Door is ");
	LCD_displayStringRowColumn(1,0,"unlocking");
	gtick=0;
	while(gtick<18);								/*delay for 15 sec and stop for 3 sec*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door is ");
	LCD_displayStringRowColumn(1,0,"locking");
	gtick=0;
	while(gtick<15);								/*delay for 15 sec*/
	LCD_clearScreen();
	Timer1_deInit();								/*deactivate timer*/
}




int main(void)
{
	unsigned char j=0,check,val;
	ECU_init();														/*initialize all driver and configurations*/
	while(1)
	{
		i=0;
		while(step==0)
			/*step 0   (enter the first pass and send it to the controller) */
		{
			LCD_displayStringRowColumn(0,0,"plz enter pass:");
			LCD_moveCursor(1,i);
			Get_Send_Pass();
			if(key == '=' && i == 5)		/*make '=' as enter*/
			{
				LCD_clearScreen();
				i=0;
				step = 1;					/*go to the second step*/
			}
			_delay_ms(500);
		}


		while(step == 1)
			/*second step	enter the second pass and send it to the controller
					to be compared*/
		{
			LCD_displayStringRowColumn(0,0,"plz re-enter the");
			LCD_displayStringRowColumn(1,0,"same pass:");
			LCD_moveCursor(2,i);
			Get_Send_Pass();
			if(key == '=' && i == 5)
			{
				while(!UART_recieveByte());
				check = UART_recieveByte();		/*check either the pass is correct or wrong*/
				if(check == Correct_Pass)		/*pass is correct so go to the third step*/
				{
					LCD_clearScreen();
					step=2;
					i=0;
				}
				else if(check == Wrong_Pass)	/*pass is wrong so go to the first step again*/
				{
					step = 0;
					i=0;
					LCD_clearScreen();
				}

			}
			_delay_ms(500);
		}

		while(step == 2)
		{
			/*fourth step	(choose either to open the door or change the password*/
			LCD_displayStringRowColumn(0,0,"+ :open the door");
			LCD_displayStringRowColumn(1,0,"- :change pass");
			key = KEYPAD_getPressedKey();
			val = key;
			if(val == '+')
			{
				LCD_clearScreen();
				while(i<6)
				{
					LCD_displayStringRowColumn(0,0,"plz enter pass:");
					LCD_moveCursor(1,i);
					Get_Send_Pass();
					if(key == '=' && i == 5)				/*make '=' as enter*/
					{
						LCD_clearScreen();
						while(!UART_recieveByte());
						check = UART_recieveByte();
						if(check == Correct_Pass)			/*if pass is correct open the door*/
						{
							i++;
							LCD_clearScreen();
							step = 3;
						}
						else if(check == Wrong_Pass)		/*if pass is wrong go to step 4*/
						{
							i++;
							step = 4;
						}
					}
					_delay_ms(500);
				}
			}
			else if(val == '-')
			{
				LCD_clearScreen();
				while(i<6)
				{
					LCD_displayStringRowColumn(0,0,"plz enter pass:");
					LCD_moveCursor(1,i);
					Get_Send_Pass();
					if(key == '=' && i == 5)
					{
						LCD_clearScreen();
						while(!UART_recieveByte());
						check = UART_recieveByte();
						if(check == Correct_Pass)
							/*if pass is correct the go to the first step to change the pass*/
						{
							LCD_clearScreen();
							step=0;
							i++;
							UART_sendByte(READY);			/*send ready bit to controller*/
							UART_sendByte(STEP0);
						}
						else if(check == Wrong_Pass)		/*if pass is wrong go to step 4*/
						{
							step = 4;
							i++;

						}

					}
					_delay_ms(500);
				}
			}
		}
		while(step == 3)		/*un-lockig the door if the password is correct*/
		{
			Door();
		}
		while(step == 4)	/* if wrong pass: try again for 2 more times*/
		{
			i=0;
			j=0;
			while(j<3)
			{
				LCD_displayStringRowColumn(0,0,"plz enter pass:");
				LCD_moveCursor(1,i);
				Get_Send_Pass();
				if(key == '=' && i == 5)
				{
					LCD_clearScreen();
					while(!UART_recieveByte());
					check = UART_recieveByte();
					if(check == Correct_Pass)
						/*if pass is correct then go back to opening the door or changing
											the password*/
					{
						LCD_clearScreen();
						i=0;
						j=3;
						if(val == '+')
						{
							step=3;
						}
						else if(val == '-')
						{
							step=0;
							UART_sendByte(READY);			/*send ready bit to controller*/
							UART_sendByte(STEP0);			/*go to step0 to change the password*/
						}
					}
					/*if pass is wrong again go activate the buzzer in the controller*/
					else if(check == Wrong_Pass)
					{
						j++;
						i=0;
						if(j==2)
						{
							i=0;
							j++;
							step = 5;
							LCD_clearScreen();
						}
					}
				}
				_delay_ms(500);
			}
		}
		while(step == 5)
		{
			Buzz();			/*activate the buzzer*/
			i=0;
		}
	}
}

