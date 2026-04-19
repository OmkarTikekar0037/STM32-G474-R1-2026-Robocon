/*
 * mpu.h
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */

#ifndef INC_MPU_H_
#define INC_MPU_H_
#include"main.h"
//--------------Public API------------

void MPU_Init(void);
void MPU_Calibrate(void);
void MPU_Update(void);

// outputs
extern float yaw;


#endif /* INC_MPU_H_ */
