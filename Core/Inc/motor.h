/*
 * motor.h
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */


#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include <stdint.h>

/* -------- Motor identifiers -------- */
typedef enum
{
    MOTOR_FL = 0,
    MOTOR_FR,
    MOTOR_BL,
    MOTOR_BR
} MotorID;

/* -------- Public API -------- */
void Motor_Init(void);
void Motor_SetSpeed(MotorID motor, int16_t speed);
void Motor_StopAll(void);

#endif /* INC_MOTOR_H_ */


