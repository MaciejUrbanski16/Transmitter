#include "accelerometer.h"
#include <stdio.h>

const float g = 9.803;
const int rawGrawity = 5450;


State accelerationDataReadingIndicator = NONE;

RawAcceleration rawAccelerationBuffer[sizeOfBufferForRawData];
uint16_t indexInRawAccelerationBuffer = 0;

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
	accelerationDataReadingIndicator = READING_ACCELERATION;
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
	if(READING_ACCELERATION == accelerationDataReadingIndicator)
	{
		RawAcceleration rawAcceleration;
		uint8_t DataAcc[6];
		HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x28, 1, DataAcc, 6, 1);
		rawAcceleration.xRaw = (DataAcc[1]<<8) | DataAcc[0];
		rawAcceleration.yRaw = (DataAcc[3]<<8) | DataAcc[2];
		rawAcceleration.zRaw = (DataAcc[5]<<8) | DataAcc[4];
		return rawAcceleration;
	}
	else
	{
		//return RawAcceleration{0,0,0};
	}
}

float calculateAcceleration(const int rawAcceleration)
{
	return ((float)rawAcceleration * g)/(float)rawGrawity;
}

XYZaxisAccelerationMS2 getCalculatedAcceleration()
{
	XYZaxisAccelerationMS2 calculatedAcceleration;
	calculatedAcceleration.validAcceleration = 0;



	if(READING_ACCELERATION == accelerationDataReadingIndicator)
	{
		RawAcceleration rawAcceleration;
		rawAcceleration = readRawDataFromAccelerometer();
		rawAccelerationBuffer[indexInRawAccelerationBuffer] = rawAcceleration;
		indexInRawAccelerationBuffer++;
		if(indexInRawAccelerationBuffer == sizeOfBufferForRawData - 1)
		{
			indexInRawAccelerationBuffer = 0;
		}
		//here have to be implemented calculation of velocity
	}
	else if(AVERAGING_ACCELERATION == accelerationDataReadingIndicator)
	{
		RawAcceleration averagedRawAcceleration = averageRawAcceleration();

		float xAccelerationInMS2 = calculateAcceleration(averagedRawAcceleration.xRaw);
		float yAccelerationInMS2 = calculateAcceleration(averagedRawAcceleration.yRaw);
		float zAccelerationInMS2 = calculateAcceleration(averagedRawAcceleration.zRaw);


		calculatedAcceleration.xAcc = getAcceleration(xAccelerationInMS2);
		calculatedAcceleration.yAcc = getAcceleration(yAccelerationInMS2);
		calculatedAcceleration.zAcc = getAcceleration(zAccelerationInMS2);
		calculatedAcceleration.validAcceleration = 1;
	}

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

RawAcceleration averageRawAcceleration()
{
	long int summedRawX = 0;
	long int summedRawY = 0;
	long int summedRawZ = 0;

	uint16_t indexOfSum = 0;

	for(indexOfSum = 0; indexOfSum < sizeOfBufferForRawData - 1; indexOfSum++)
	{
		summedRawX += rawAccelerationBuffer[indexOfSum].xRaw;
		summedRawY += rawAccelerationBuffer[indexOfSum].yRaw;
		summedRawZ += rawAccelerationBuffer[indexOfSum].zRaw;
	}

	RawAcceleration averagedRawAcceleration;
	averagedRawAcceleration.xRaw = summedRawX / (sizeOfBufferForRawData - 1);
	averagedRawAcceleration.yRaw = summedRawY / (sizeOfBufferForRawData - 1);
	averagedRawAcceleration.zRaw = summedRawZ / (sizeOfBufferForRawData - 1);

	return averagedRawAcceleration;
}
