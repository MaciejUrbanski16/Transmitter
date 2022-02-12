#include "accelerometer.h"


void initAccelerometer()
{
	uint8_t array[2];
	array[0] = dataRate400Hz;
	array[1] = antiAliasing;

	//data rate 400Hz
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG4_ADDR, 1, &array[0], 1, 100);
	//anti aliasing filter 400Hz
	HAL_I2C_Mem_Write(&hi2c1, ACC_ADDRESS << 1, LIS3DSH_CTRL_REG5_ADDR, 1, &array[1], 1, 100);
}
