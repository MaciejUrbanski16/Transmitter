#ifndef magnetometer
#define magnetometer
#include "stm32f4xx_hal.h"

#define HMC5883L_ADRESS 0x1A

#define configurationA  0x78
#define configurationB 0xA0
#define mode 0x02

#define XaxisMSB 0x03
#define XaxisLSB 0x04

#define YaxisMSB 0x07
#define YaxisLSB 0x08

#define ZaxisMSB 0x05
#define ZaxisLSB 0x06

#define status 0x09

struct OrientationInSpace
{
	uint16_t xAxis;
	uint16_t yAxis;
	uint16_t zAxis;
};

void initHMC5883L();
#endif
