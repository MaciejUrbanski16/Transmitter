#include "magnetometer.h"

extern I2C_HandleTypeDef hi2c1;

//extern uint8_t configurationA;
//extern uint8_t configurationB;
//extern uint8_t mode;
//
//extern uint8_t XaxisMSB;
//extern uint8_t XaxisLSB;
//
//extern uint8_t YaxisMSB;
//extern uint8_t YaxisLSB;
//
//extern uint8_t ZaxisMSB;
//extern uint8_t ZaxisLSB;

//extern uint8_t status = 0x09;

void initHMC5883L()
{
	uint8_t configA = configurationA;
	uint8_t configB = configurationB;
	uint8_t m = mode;
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADRESS, /*mem addr*/ 0x00 , 1, &configA , 1, 100);
	HAL_Delay(100);
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADRESS, /*mem addr*/ 0x01 , 1, &configB , 1, 100);
	HAL_Delay(100);
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADRESS, /*mem addr*/ 0x02 , 1, &m , 1, 100);
	HAL_Delay(100);
}
