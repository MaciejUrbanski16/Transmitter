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

#define dataRate400Hz 0b01110111 //0x77 dataRate -> 400Hz and enable X Y Z axis
#define antiAliasing  0b01001000 // 400Hz anti aliasing filter bandwith | full scale selection 4G

extern I2C_HandleTypeDef hi2c1;

void initAccelerometer(void);




#endif /* INC_ACCELEROMETER_H_ */
