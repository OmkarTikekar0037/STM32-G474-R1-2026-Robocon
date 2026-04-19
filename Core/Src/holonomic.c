/*
 * holonomic.c
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */
#include "holonomic.h"
#include <math.h>

/* wheel angles in radians */
static const float wheel_angles[NUM_WHEELS] = {
    M_PI / 4,    // FL  +45°
   -M_PI / 4,    // FR  -45°
    3 * M_PI/4,  // RL  +135°
   -3 * M_PI/4   // RR  -135°
};

void Holonomic_Compute(float rx, float ry, float omega, int16_t *wheel_out)
{
    // -------- VECTOR CONVERSION --------
    float V = sqrtf(rx * rx + ry * ry);

    // avoid NaN when stick is zero
    float theta = 0.0f;
    if (V > 0.001f)
        theta = atan2f(ry, rx);

    // -------- WHEEL PROJECTION --------
    for (int i = 0; i < NUM_WHEELS; i++)
    {
        wheel_out[i] = V * cosf(theta - wheel_angles[i]) + omega;
    }

    // -------- NORMALIZATION --------
    float maxVal = fabsf(wheel_out[0]);

    for (int i = 1; i < NUM_WHEELS; i++)
    {
        if (fabsf(wheel_out[i]) > maxVal)
            maxVal = fabsf(wheel_out[i]);
    }

    if (maxVal > 1.0f)
    {
        for (int i = 0; i < NUM_WHEELS; i++)
            wheel_out[i] /= maxVal;
    }
}

