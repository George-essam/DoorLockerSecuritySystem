/*
 * Buzzer.h
 *
 *  Created on: Oct 31, 2022
 *      Author: georg
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

#define BuzzerPort              PORTC_ID
#define BuzzerPin   			PIN2_ID

void Buzzer_Init(void);

void Buzzer_On(void);

void Buzzer_Off(void);


#endif /* BUZZER_H_ */
