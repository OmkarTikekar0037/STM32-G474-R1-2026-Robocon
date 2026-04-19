/*
 * motor.c
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */
#include "motor.h"
#include <stdlib.h>
/*
 * IMPORTANT:
 * These must match your CubeMX configuration
 * (same pins and timers you used earlier)
 */

/* -------- Direction pins -------- */
#define FL_DIR_PORT GPIOB
#define FL_DIR_PIN  GPIO_PIN_3

#define FR_DIR_PORT GPIOB
#define FR_DIR_PIN  GPIO_PIN_5

#define BL_DIR_PORT GPIOB
#define BL_DIR_PIN  GPIO_PIN_14

#define BR_DIR_PORT GPIOC
#define BR_DIR_PIN  GPIO_PIN_6

/* -------- PWM handles (from main.c) -------- */
extern TIM_HandleTypeDef htim8; // FL
extern TIM_HandleTypeDef htim16;  // FR
extern TIM_HandleTypeDef htim2; // BL
extern TIM_HandleTypeDef htim15;  // BR

/* -------- PWM channels -------- */
#define FL_PWM_TIMER &htim8
#define FL_PWM_CH    TIM_CHANNEL_1

#define FR_PWM_TIMER &htim16
#define FR_PWM_CH    TIM_CHANNEL_1

#define BL_PWM_TIMER &htim2
#define BL_PWM_CH    TIM_CHANNEL_4

#define BR_PWM_TIMER &htim15
#define BR_PWM_CH    TIM_CHANNEL_2

/* -------- Internal helper -------- */
static void Motor_Write(GPIO_TypeDef *port, uint16_t pin,
                        TIM_HandleTypeDef *htim, uint32_t channel,
                        int16_t speed)
{
    GPIO_PinState dir = (speed >= 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    uint16_t pwm = (uint16_t)abs(speed);

    HAL_GPIO_WritePin(port, pin, dir);
    __HAL_TIM_SET_COMPARE(htim, channel, pwm);
}

/* -------- Init -------- */
void Motor_Init(void)
{
    /* Start PWM channels */
    HAL_TIM_PWM_Start(FL_PWM_TIMER, FL_PWM_CH);
    HAL_TIM_PWM_Start(FR_PWM_TIMER, FR_PWM_CH);
    HAL_TIM_PWM_Start(BL_PWM_TIMER, BL_PWM_CH);
    HAL_TIM_PWM_Start(BR_PWM_TIMER, BR_PWM_CH);
}

/* -------- Set individual motor -------- */
void Motor_SetSpeed(MotorID motor, int16_t speed)
{
    switch (motor)
    {
        case MOTOR_FL:
            Motor_Write(FL_DIR_PORT, FL_DIR_PIN,
                        FL_PWM_TIMER, FL_PWM_CH, speed);
            break;

        case MOTOR_FR:
            Motor_Write(FR_DIR_PORT, FR_DIR_PIN,
                        FR_PWM_TIMER, FR_PWM_CH, speed);
            break;

        case MOTOR_BL:
            Motor_Write(BL_DIR_PORT, BL_DIR_PIN,
                        BL_PWM_TIMER, BL_PWM_CH, speed);
            break;

        case MOTOR_BR:
            Motor_Write(BR_DIR_PORT, BR_DIR_PIN,
                        BR_PWM_TIMER, BR_PWM_CH, speed);
            break;
    }
}

/* -------- Stop all motors -------- */
void Motor_StopAll(void)
{
    Motor_SetSpeed(MOTOR_FL, 0);
    Motor_SetSpeed(MOTOR_FR, 0);
    Motor_SetSpeed(MOTOR_BL, 0);
    Motor_SetSpeed(MOTOR_BR, 0);
}

