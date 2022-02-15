#include "accelerometer.h"

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
		  HAL_Delay(500);

	  }
}

float calculateAcceleration(const int rawAcceleration)
{
	return ((float)rawAcceleration * g)/(float)rawGrawity;
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
