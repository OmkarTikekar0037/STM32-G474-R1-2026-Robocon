/*
 * yaw_control.h
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */

#ifndef INC_YAW_CONTROL_H_
#define INC_YAW_CONTROL_H_
#include "main.h"
// public APIs

void YawControl_Init(void);
int16_t YawControl_Update(int lx, int ry, int rx);

int YawControl_GetOmega(void);


#endif /* INC_YAW_CONTROL_H_ */
