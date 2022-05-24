#include "accelerometer.h"
#include <stdio.h>

const float g = 9.803;
const int rawGrawity = 5450;
extern UART_HandleTypeDef huart2;

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
	char OK[] = "initACC READ";
	lcdClear();
	lcdSetCursor(0, 0);
	lcdSendString(OK);
	HAL_Delay(1000);
}

void waitTillAccelerometerIsInitialized(void)
{
	  while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)ACC_ADDRESS << 1, 10, 100) != HAL_OK)
	  {
		  uint8_t who = 8;
		  char outWho[16];
		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x0F, 1, &who, 1, 100);
		  sprintf(outWho, "AC2not %d", who);
		  lcdClear();
		  lcdSetCursor(0, 0);
		  lcdSendString(outWho);

		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  HAL_Delay(200);

	  }
}

RawAcceleration readRawDataFromAccelerometer()
{
//	if(READING_ACCELERATION == accelerationDataReadingIndicator)
//	{
		RawAcceleration rawAcceleration;
		uint8_t DataAcc[6];
		HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x28, 1, DataAcc, 6, 1);
		rawAcceleration.xRaw = (DataAcc[1]<<8) | DataAcc[0]; //0x28 0x29
		rawAcceleration.yRaw = (DataAcc[3]<<8) | DataAcc[2]; //0x2A 0x2B
		rawAcceleration.zRaw = (DataAcc[5]<<8) | DataAcc[4]; //0x2C 0x2D
			char out[10];
		  sprintf(out, "Z r %d", rawAcceleration.zRaw);
		  lcdClear();
		  lcdSetCursor(0, 0);
		  lcdSendString(out);
		  	 HAL_Delay(600);
		return rawAcceleration;
//	}
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
	}
	else if(AVERAGING_ACCELERATION == accelerationDataReadingIndicator)
	{
		//RawAcceleration averagedRawAcceleration = averageRawAcceleration();

		RawAcceleration rawAcceleration;
		rawAcceleration = readRawDataFromAccelerometer();

		float xAccelerationInMS2 = calculateAcceleration(rawAcceleration.xRaw);
		float yAccelerationInMS2 = calculateAcceleration(rawAcceleration.yRaw);
		float zAccelerationInMS2 = calculateAcceleration(rawAcceleration.zRaw);

		int xScaledAcc = (int)(xAccelerationInMS2 * 1000.0f);
		int yScaledAcc = (int)(yAccelerationInMS2 * 1000.0f);
		int zScaledAcc = (int)(zAccelerationInMS2 * 1000.0f);

		calculatedAcceleration.xScaledAcc = xScaledAcc;
		calculatedAcceleration.yScaledAcc = yScaledAcc;
		calculatedAcceleration.zScaledAcc = zScaledAcc;


		calculatedAcceleration.xAcc = getAcceleration(xAccelerationInMS2);
		calculatedAcceleration.yAcc = getAcceleration(yAccelerationInMS2);
		calculatedAcceleration.zAcc = getAcceleration(zAccelerationInMS2);
		calculatedAcceleration.validAcceleration = 1;

//		char out[10];
//	  sprintf(out, "C r %d", calculatedAcceleration.zAcc);
//	  lcdClear();
//	  lcdSetCursor(0, 0);
//	  lcdSendString(out);
//	  HAL_Delay(600);
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
