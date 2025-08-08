/*
 * dc_motor.h
 *
 *  Created on: Oct 8, 2022
 *      Author: georg
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define motorout1_portID	PORTD_ID
#define motorout1_pinID 	PIN2_ID
#define motorout2_portID	PORTD_ID
#define motorout2_pinID		PIN3_ID

void DcMotor_Init(void);

void DcMotor_Rotate(uint8 state,uint8 speed);

#endif /* DC_MOTOR_H_ */
