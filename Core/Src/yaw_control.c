/*
 * yaw_control.c
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */
#include "yaw_control.h"
#include "mpu.h"
#include "stdlib.h"   // for abs()

/* ---------------- INTERNAL STATE ---------------- */

static float targetYaw = 0.0f;
static float prevError = 0.0f;
static float integral  = 0.0f;

static uint32_t prevTime = 0;

static float kp = 1.7f;
static float ki = 0.09f;
static float kd = 0.7f;

static int omega_out = 0;

/* ---------------- INIT ---------------- */

void YawControl_Init(void)
{
    targetYaw = yaw;   // lock initial heading
    prevError = 0.0f;
    integral  = 0.0f;
    prevTime  = HAL_GetTick();
}

/* ---------------- UPDATE ---------------- */

int16_t YawControl_Update(int lx, int ry, int rx)
{
    uint32_t now = HAL_GetTick();
    float dt = (now - prevTime) / 1000.0f;
    prevTime = now;

    // safety clamp
    if (dt <= 0.0f || dt > 0.5f) dt = 0.01f;

    /* -------- MODE DETECTION -------- */

    uint8_t rotating    = (abs(lx) > 8);
    uint8_t translating = (abs(rx) > 5 || abs(ry) > 5);

    /* -------- ROTATION MODE -------- */
    if (rotating)
    {
        // user has control
        omega_out = lx;

        // reset stabilization
        targetYaw = yaw;
        integral  = 0.0f;
        prevError = 0.0f;

        return omega_out;
    }

    /* -------- TRANSLATION MODE -------- */
    if (translating)
    {
        float error = targetYaw - yaw;

        // wrap angle [-180, 180]
        if (error > 180.0f)  error -= 360.0f;
        if (error < -180.0f) error += 360.0f;

        integral += error * dt;

        float derivative = (error - prevError) / dt;

        float output = (kp * error) + (ki * integral) + (kd * derivative);

        omega_out = -(int)output;

        prevError = error;
    }
    else
    {
        /* -------- IDLE MODE -------- */
        targetYaw = yaw;
        integral  = 0.0f;
        prevError = 0.0f;
        omega_out = 0;
    }
}

/* ---------------- OUTPUT ---------------- */

int YawControl_GetOmega(void)
{
    return omega_out;
}


