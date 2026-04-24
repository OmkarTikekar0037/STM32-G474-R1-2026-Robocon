/*
 * mpu.c
 *
 *  Created on: Apr 18, 2026
 *      Author: F.R.I.D.A.Y
 */

#include "mpu.h"
#include "main.h"
#include "math.h"

/* ===== MPU Registers ===== */
#define MPU_ADDR        0xD0
#define PWR_MGMT_1      0x6B
#define ACCEL_XOUT_H    0x3B
#define GYRO_ZOUT_H     0x47

/* ===== Public Output ===== */
float yaw = 0.0f;

/* ===== Internal Variables ===== */
static float yaw_gyro = 0.0f;
float gyro_z_offset = 0.0f;

static uint32_t imu_prev = 0;

/* ===== Yaw Hold Variables ===== */
static float heldYaw = 0.0f;
static uint8_t yawLocked = 0;

static float prevYaw = 0.0f;
static uint16_t stableCount = 0;

/* ===== Low-level helpers ===== */

static void writeRegister(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c2, MPU_ADDR, reg, 1, &data, 1, 100);
}

static void readRegisters(uint8_t reg, uint8_t count, uint8_t *data)
{
    HAL_I2C_Mem_Read(&hi2c2, MPU_ADDR, reg, 1, data, count, 100);
}

/* ===== Public Functions ===== */

void MPU_Init(void)
{
    HAL_Delay(100);
    writeRegister(PWR_MGMT_1, 0x00);
    imu_prev = HAL_GetTick();
}

void MPU_Calibrate(void)
{
    float sum = 0;
    uint8_t data[2];

    for (int i = 0; i < 500; i++)
    {
        readRegisters(GYRO_ZOUT_H, 2, data);

        int16_t gz = (data[0] << 8) | data[1];
        sum += gz / 131.0f;

        HAL_Delay(4);
    }

    gyro_z_offset = sum / 500.0f;
}

void MPU_Update(void)
{
    uint8_t rawData[14];

    readRegisters(ACCEL_XOUT_H, 14, rawData);

    int16_t gz = (rawData[12] << 8) | rawData[13];
    float Gz = (gz / 131.0f) - gyro_z_offset;

    uint32_t now = HAL_GetTick();
    float dt = (now - imu_prev) / 1000.0f;
    imu_prev = now;

    if (dt <= 0 || dt > 0.1f)
        dt = 0.01f;

    /* ---- Yaw Integration ---- */
    yaw_gyro += Gz * dt;
    float currentYaw = yaw_gyro;

    /* ---- Yaw Hold Logic ---- */
    float diff = fabsf(currentYaw - prevYaw);

    if (diff < 0.03f)   // threshold (tune if needed)
    {
        stableCount++;
    }
    else
    {
        stableCount = 0;
        yawLocked = 0;
    }

    if (stableCount > 50 && !yawLocked)
    {
        heldYaw = currentYaw;
        yawLocked = 1;
    }

    prevYaw = currentYaw;

    /* ---- Final Output ---- */
    if (yawLocked)
        yaw = heldYaw;
    else
        yaw = currentYaw;
}
