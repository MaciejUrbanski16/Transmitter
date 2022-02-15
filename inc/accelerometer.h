/*
 * accelerometer.h
 *
 *  Created on: 12.02.2022
 *      Author: Admin
 */

#ifndef INC_ACCELEROMETER_H_
#define INC_ACCELEROMETER_H_

#include "stm32f4xx_hal.h"

#define ACC_ADDRESS 0x1E //0b000111100

#define LIS3DSH_CTRL_REG4_ADDR               0x20
#define LIS3DSH_CTRL_REG5_ADDR               0x24
#define LIS3DSH_CTRL_REG3_ADDR               0x23
#define LIS3DSH_CTRL_REG1_ADDR               0x21

#define dataRate400Hz    0b00110111 //0x77 dataRate -> 400Hz and enable X Y Z axis
#define antiAliasing     0b00010000 // 12,5Hz anti aliasing filter bandwith | full scale selection 4G
#define interruptsEnable 0b11101000
#define sm1Control       0b00001001

extern const float g;
extern const int rawGrawity;

typedef struct
{
	int integerPart;
	int floatingPart;
}AccelerationMS2;

typedef struct
{
	AccelerationMS2 xAcc;
	AccelerationMS2 yAcc;
	AccelerationMS2 zAcc;
}XYZaxisAccelerationMS2;

extern I2C_HandleTypeDef hi2c1;

void initAccelerometer(void);
void waitTillAccelerometerIsInitialized(void);

int readRawX();
int readRawY();
int readRawZ();

float calculateAcceleration(const int rawAcceleration);

XYZaxisAccelerationMS2 getCalculatedAcceleration();

AccelerationMS2 getAcceleration(const float floatingAcceleration);




#endif /* INC_ACCELEROMETER_H_ */
