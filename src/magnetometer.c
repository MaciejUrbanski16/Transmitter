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
	uint8_t array[2];
	array[0] = 1;
	array[1] = 0x1D;

	HAL_I2C_Mem_Write(&hi2c1, 0x1A, 0x0B, 1, &array[0], 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, 0x1A, 0x09, 1, &array[1], 1, 100);
}
