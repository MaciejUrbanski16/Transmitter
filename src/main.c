///**
//  ******************************************************************************
//  * @file    main.c
//  * @author  Ac6
//  * @version V1.0
//  * @date    01-December-2013
//  * @brief   Default main function.
//  ******************************************************************************
//*/

#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"

#include <string.h>
#include "stm32f4xx.h"
#include "i2c-lcd.h"
#include "magnetometer.h"
#include "accelerometer.h"

#define RS_PORT GPIOA
#define RS_PIN GPIO_PIN_1
#define RW_PORT GPIOA
#define RW_PIN GPIO_PIN_2
#define EN_PORT GPIOA
#define EN_PIN GPIO_PIN_3
#define D4_PORT GPIOA
#define D4_PIN GPIO_PIN_4
#define D5_PORT GPIOA
#define D5_PIN GPIO_PIN_5
#define D6_PORT GPIOA
#define D6_PIN GPIO_PIN_6
#define D7_PORT GPIOA
#define D7_PIN GPIO_PIN_7


enum I2C_services
{
	WRITE_TO_LCD = 0,
	READ_FROM_MAGNETOMETER,
	READ_FROM_AKCELEROMETER,
	NONE
};

volatile int currentI2CService = NONE;
UART_HandleTypeDef huart2;

I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef timer2, timer4;


void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);

//TIM2_IRQn


int row=0;
int col=0;
volatile int counterTimer4 = 0;

static const uint16_t bufferAccSize = 1024;
volatile int16_t bufferAccXaxis[1024];
volatile int16_t bufferAccYaxis[1024];
volatile int16_t bufferAccZaxis[1024];

void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer2);
}

void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		  int16_t Xaxis = 0;
		  int16_t Yaxis = 0;
		  int16_t Zaxis = 0;
		  uint8_t DataAccX[2];
		  uint8_t DataAccY[2];
		  uint8_t DataAccZ[2];
		  uint8_t DataAccT[2];
		  int16_t rawX, rawY, rawZ, rawT;

		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x28, 1, DataAccX, 2, 100);
		  rawX = (DataAccX[1]<<8) | DataAccX[0];

		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x2A, 1, DataAccY, 2, 100);
		  rawY = (DataAccY[1]<<8) | DataAccY[0];

		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x2C, 1, DataAccZ, 2, 100);
		  rawZ = (DataAccZ[1]<<8) | DataAccZ[0];

		  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x0C, 1, DataAccT, 2, 100);
		  rawT = (DataAccT[1]<<8) | DataAccT[0];

	}
	else if(htim->Instance == TIM4)
	{
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}




int main(void)
{


	SystemCoreClock = 8000000;	// taktowanie 8Mhz

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__PWR_CLK_ENABLE();



	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	HAL_Init();
	GPIO_InitTypeDef GPIO_InitStruct;


	 __GPIOA_CLK_ENABLE();
	 __GPIOB_CLK_ENABLE();
	 __GPIOC_CLK_ENABLE();
	 __USART2_CLK_ENABLE();

	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_1| GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7/*|GPIO_PIN_8*/;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitTypeDef gpio_I2C1_SDA_SCL;
	gpio_I2C1_SDA_SCL.Pin = /*GPIO_PIN_4 |*/GPIO_PIN_8 | GPIO_PIN_9;
	gpio_I2C1_SDA_SCL.Mode = GPIO_MODE_AF_OD;
			// SCL, SDA
	gpio_I2C1_SDA_SCL.Pull = GPIO_NOPULL;
	gpio_I2C1_SDA_SCL.Alternate = GPIO_AF4_I2C1;
	gpio_I2C1_SDA_SCL.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_I2C1_SDA_SCL);

	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

	MX_I2C1_Init();
	MX_TIM4_Init();
	MX_TIM2_Init();
    MX_USART2_UART_Init();




    initAccelerometer();

	  lcd_init ();
	  initHMC5883L();
	  lcd_clear();


	  char dataXY[16];
	  char dataZ[16];
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


	  while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)HMC5883L_ADRESS, 10, 100) != HAL_OK)
	  {
		  lcd_clear();
		  lcd_put_cur(0, 0);
		  lcd_send_string ("HMCnotready");

		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  HAL_Delay(500);

	  }



	  uint8_t dataM[6];
	  int8_t DataY[2];
	  int16_t Xaxis = 0;
	  int16_t Yaxis = 0;
	  int16_t Zaxis = 0;
//	  uint8_t DataAccX[2];
//	  uint8_t DataAccY[2];
//	  uint8_t DataAccZ[2];
//	  uint8_t DataAccT[2];
//	  int16_t rawX, rawY, rawZ, rawT;


	  while (1)
	  {
		  HAL_I2C_Mem_Read(&hi2c1, HMC5883L_ADRESS, 0x06, 1, dataM, 1, 100);
		  if((dataM[0]&0x01)==1)
		  {
			  HAL_I2C_Mem_Read(&hi2c1, HMC5883L_ADRESS, 0x00, 1, dataM, 6, 100);
			  Xaxis= (dataM[1]<<8) | dataM[0];
			  Yaxis= (dataM[3]<<8) | dataM[2];
			  Zaxis= (dataM[5]<<8) | dataM[4];

//			  float degree = atan2f((float)Yaxis, (float)Xaxis)*(180/M_PI);
//			  HAL_Delay(50);
//			  lcd_clear ();
//			  lcd_put_cur(0, 0);
//			  sprintf(dataXY, "Degr:%d", (int)degree);
//			  lcd_send_string (dataXY);
//			  lcd_put_cur(1, 0);
//			  sprintf(dataZ, "Z: %d", Zaxis);
//			  lcd_send_string (dataZ);

			  uint8_t who = 8;
			  char acc[16];
			  char acc2[16];
//			  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x28, 1, DataAccX, 2, 100);
//			  rawX = (DataAccX[1]<<8) | DataAccX[0];
//
//			  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x2A, 1, DataAccY, 2, 100);
//			  rawY = (DataAccY[1]<<8) | DataAccY[0];
//
//			  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x2C, 1, DataAccZ, 2, 100);
//			  rawZ = (DataAccZ[1]<<8) | DataAccZ[0];
//
//			  HAL_I2C_Mem_Read(&hi2c1, ACC_ADDRESS << 1, 0x0C, 1, DataAccT, 2, 100);
//			  rawT = (DataAccT[1]<<8) | DataAccT[0];

//			  sprintf(acc, "X %d Y %d", rawX, rawY);
//
//			  lcd_clear();
//			  lcd_put_cur(0, 0);
//			  lcd_send_string(acc);
//
//			  lcd_put_cur(1, 0);
//			  sprintf(acc2, "Z %d T1 %d", rawZ, rawT);
//			  lcd_send_string(acc2);
		  }

		  HAL_Delay(1000);

		  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	  }

}

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);

}

static void MX_TIM2_Init(void)
{
	timer2.Instance = TIM2;
	timer2.Init.Period = 500 - 1;
	timer2.Init.Prescaler = 8000 - 1;
	timer2.Init.ClockDivision = 0;
	timer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer2.Init.RepetitionCounter = 0;
	timer2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&timer2);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&timer2);
}

static void MX_TIM4_Init(void)
{
	timer4.Instance = TIM4;
	timer4.Init.Period = 200 - 1;
	timer4.Init.Prescaler = 8000 - 1;
	timer4.Init.ClockDivision = 0;
	timer4.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer4.Init.RepetitionCounter = 0;
	timer4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&timer4);

	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_TIM_Base_Start_IT(&timer4);
}


void MX_USART2_UART_Init(void)
{
//	huart2.Instance = USART2;
//	huart2.Init.BaudRate = 115200;
//	huart2.Init.WordLength = UART_WORDLENGTH_8B;
//	huart2.Init.StopBits = UART_STOPBITS_1;
//	huart2.Init.Parity = UART_PARITY_NONE;
//	huart2.Init.Mode = UART_MODE_TX_RX;
//	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;


	 huart2.Instance = USART2;
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  HAL_UART_Init(&huart2);
}

