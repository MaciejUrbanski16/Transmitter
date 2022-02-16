#include "accelerometer.h"
#include <stdio.h>

const float g = 9.803;
const int rawGrawity = 5450;

void initAccelerometer()
{
	uint8_t array[4];
	array[0] = dataRate400Hz;
	array[1] = antiAliasing;
	array[2] = interruptsEnable;
	array[3] = sm1Control;

	//data rate 12,5Hz
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG4_ADDR, 1, &array[0], 1, 100);
	//anti aliasing filter 400Hz
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG5_ADDR, 1, &array[1], 1, 100);
	//interrupts enabled
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG3_ADDR, 1, &array[2], 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG1_ADDR, 1, &array[3], 1, 100);
	//HAL_I2C_Mem_Write_IT(&hi2c1,ACC_ADDRESS << 1, LIS3DSH_CTRL_REG4_ADDR, 1, &array[0], 1);
}

void waitTillAccelerometerIsInitialized(void)
{
	  while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)ACC_ADDRESS << 1, 10, 100) != HAL_OK)
	  {
		  uint8_t who = 8;
		  char outWho[16];
		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x0F, 1, &who, 1, 100);
		  sprintf(outWho, "AC2not %d", who);
		  lcd_clear();
		  lcd_put_cur(0, 0);
		  lcd_send_string(outWho);

		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  HAL_Delay(200);

	  }
}

RawAcceleration readRawDataFromAccelerometer()
{
	RawAcceleration rawAcceleration;
	uint8_t DataAcc[6];
	HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x28, 1, DataAcc, 6, 1);
	rawAcceleration.xRaw = (DataAcc[1]<<8) | DataAcc[0];
	rawAcceleration.yRaw = (DataAcc[3]<<8) | DataAcc[2];
	rawAcceleration.zRaw = (DataAcc[5]<<8) | DataAcc[4];
	return rawAcceleration;
}

float calculateAcceleration(const int rawAcceleration)
{
	return ((float)rawAcceleration * g)/(float)rawGrawity;
}

XYZaxisAccelerationMS2 getCalculatedAcceleration()
{
	XYZaxisAccelerationMS2 calculatedAcceleration;
	RawAcceleration rawAcceleration;

	rawAcceleration = readRawDataFromAccelerometer();

	float xAccelerationInMS2 = calculateAcceleration(rawAcceleration.xRaw);
	float yAccelerationInMS2 = calculateAcceleration(rawAcceleration.yRaw);
	float zAccelerationInMS2 = calculateAcceleration(rawAcceleration.zRaw);

	calculatedAcceleration.xAcc = getAcceleration(xAccelerationInMS2);
	calculatedAcceleration.yAcc = getAcceleration(yAccelerationInMS2);
	calculatedAcceleration.zAcc = getAcceleration(zAccelerationInMS2);

	return calculatedAcceleration;
}

AccelerationMS2 getAcceleration(const float floatingAcceleration)
{
	AccelerationMS2 acceleration;

	int integerPart = (int)floatingAcceleration;
	int floatingPart = (int)((float)(floatingAcceleration - integerPart) * 1000);

	acceleration.integerPart = integerPart;
	acceleration.floatingPart = floatingPart;

	return acceleration;
}
