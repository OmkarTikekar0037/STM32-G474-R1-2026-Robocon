#ifndef HOLONOMIC_H
#define HOLONOMIC_H

#include <stdint.h>

// number of wheels
#define NUM_WHEELS 4

// output array index mapping
#define WHEEL_FL 0
#define WHEEL_FR 1
#define WHEEL_RL 2
#define WHEEL_RR 3

// API
void Holonomic_Compute(float rx, float ry, float omega, int16_t *wheel_out);

#endif
