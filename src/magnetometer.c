#include "magnetometer.h"
#include <math.h>

extern I2C_HandleTypeDef hi2c1;

void initHMC5883L()
{
	uint8_t array[2];
	array[0] = 1;
	array[1] = 0x1D;

	HAL_I2C_Mem_Write(&hi2c1, 0x1A, 0x0B, 1, &array[0], 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, 0x1A, 0x09, 1, &array[1], 1, 100);
}

void waitTillMagnetometerIsInitialized()
{
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)HMC5883L_ADRESS, 10, 100) != HAL_OK)
	{
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string ("HMCnotready");

		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(500);
	}
}

uint8_t checkAvalibilityOfDataInRegister()
{
	uint8_t someOutput[6];
	HAL_I2C_Mem_Read(&hi2c1, HMC5883L_ADRESS, 0x06, 1, someOutput, 6, 100);

	if((someOutput[0] & 0x01) == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

float calculateAzimutWithDegree()
{
	OrientationInSpace orientationInSpace = readRawDataFromMagnetometer();

//DEBUG
//	char acc2[16];
//	  lcd_clear ();
//	  lcd_put_cur(0, 0);
//	  sprintf(acc2, "X:%dY:%dZ:%d", orientationInSpace.xAxis, orientationInSpace.yAxis, orientationInSpace.zAxis);
//	  lcd_send_string(acc2);
//	  HAL_Delay(800);
	return atan2f((float)orientationInSpace.yAxis, (float)orientationInSpace.xAxis)*(180.0/M_PI);
}

OrientationInSpace readRawDataFromMagnetometer()
{
	OrientationInSpace rawOrientationInSpace;
	uint8_t dataM[6];

	HAL_I2C_Mem_Read(&hi2c1, HMC5883L_ADRESS, 0x00, 1, dataM, 6, 100);
	rawOrientationInSpace.xAxis = (dataM[1]<<8) | dataM[0];
	rawOrientationInSpace.yAxis = (dataM[3]<<8) | dataM[2];
	rawOrientationInSpace.zAxis = (dataM[5]<<8) | dataM[4];

	return rawOrientationInSpace;
}
