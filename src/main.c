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
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"

#include <string.h>
#include "stm32f4xx.h"
#include "i2c-lcd.h"
#include "magnetometer.h"

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

I2C_HandleTypeDef hi2c1, hi2c3;
TIM_HandleTypeDef timer4;


void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM4_Init(void);

int row=0;
int col=0;
volatile int counterTimer4 = 0;

void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
//		char dataToLCD[32];
//		  sprintf(dataToLCD, "Counter tim4 :%d", counterTimer4);
//		  lcd_clear();
//		  lcd_put_cur(0, 0);
//		  lcd_send_string (dataToLCD);
//		  counterTimer4++;
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
void initHMC5883L()
{
//	uint8_t configA = configurationA;
//	uint8_t configB = configurationB;
//	uint8_t m = mode;
//	HAL_I2C_Mem_Write(&hi2c3, HMC5883L_ADRESS, /*mem addr*/ 0x00 , 1, &configA , 1, 100);
//	HAL_Delay(100);
//	HAL_I2C_Mem_Write(&hi2c3, HMC5883L_ADRESS, /*mem addr*/ 0x01 , 1, &configB , 1, 100);
//	HAL_Delay(100);
//	HAL_I2C_Mem_Write(&hi2c3, HMC5883L_ADRESS, /*mem addr*/ 0x02 , 1, &m , 1, 100);
//	HAL_Delay(100);
	uint8_t array[2];
	array[0] = 1;
	array[1] = 0x11;

	HAL_I2C_Mem_Write(&hi2c3, 0x1A, 0x0B, 1, &array[0], 1, 100);
	HAL_I2C_Mem_Write(&hi2c3, 0x1A, 0x09, 1, &array[1], 1, 100);

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

	    __HAL_RCC_I2C1_CLK_ENABLE();
	    __HAL_RCC_GPIOD_CLK_ENABLE();
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_1| GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7/*|GPIO_PIN_8*/;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitTypeDef gpio1;
	gpio1.Pin = /*GPIO_PIN_4 |*/GPIO_PIN_8 | GPIO_PIN_9;
	gpio1.Mode = GPIO_MODE_AF_OD;
			// SCL, SDA
	gpio1.Pull = GPIO_NOPULL;
	gpio1.Alternate = GPIO_AF4_I2C1;
	gpio1.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio1);

	GPIO_InitTypeDef gpio2;
		gpio2.Pin = /*GPIO_PIN_4 |*/GPIO_PIN_9;
		gpio2.Mode = GPIO_MODE_AF_OD;
				// SCL, SDA
		gpio2.Pull = GPIO_NOPULL;
		gpio2.Alternate = GPIO_AF4_I2C1;
		gpio2.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOC, &gpio2);

		gpio2.Pin = GPIO_PIN_8;
		gpio2.Mode = GPIO_MODE_AF_OD;
		HAL_GPIO_Init(GPIOA, &gpio2);

	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C3_CLK_ENABLE();

	__HAL_RCC_TIM4_CLK_ENABLE();
	MX_I2C1_Init();
	MX_I2C3_Init();


	MX_TIM4_Init();



    MX_USART2_UART_Init();






	  lcd_init ();
	  initHMC5883L();
	  lcd_clear();


	  char dataX[16];
	  char dataY[16];

	  while (HAL_I2C_IsDeviceReady(&hi2c3, (uint16_t)HMC5883L_ADRESS, 10, 100) != HAL_OK)
	  {
		  lcd_clear();
		  lcd_put_cur(0, 0);
		  lcd_send_string ("I2C8tw. HMC");

		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  HAL_Delay(500);

	  }

#define HMC5883l_ADD_DATAX_MSB_MULTI (XaxisMSB | 0x80)
#define HMC5883l_ADD_DATAY_MSB_MULTI (YaxisMSB | 0x80)
	  uint8_t dataM[6];
	  uint8_t DataY[2];
	  uint16_t Xaxis = 0;
	  uint16_t Yaxis = 0;
	  uint16_t Zaxis = 0;


	  while (1)
	  {
		  //HAL_I2C_Mem_Read(&hi2c3,HMC5883L_ADRESS,HMC5883l_ADD_DATAX_MSB_MULTI,1,DataX,2,100);
		  HAL_I2C_Mem_Read(&hi2c3, HMC5883L_ADRESS, 0x06, 1, dataM, 1, 100);
		  if((dataM[0]&0x01)==1)
		  {
			  HAL_I2C_Mem_Read(&hi2c3, HMC5883L_ADRESS, 0x00, 1, dataM, 6, 100);
			  Xaxis= (dataM[1]<<8) | dataM[0];
			  Yaxis= (dataM[3]<<8) | dataM[2];
			  Zaxis= (dataM[5]<<8) | dataM[4];

			  HAL_Delay(50);
			  lcd_clear ();
			  lcd_put_cur(0, 0);
			  sprintf(dataX, "Xd10: %d", 76);
			  lcd_send_string (dataX);
		  }

//		  Xaxis = ((DataX[1]<<8) | DataX[0])/660.f;
//		  HAL_I2C_Mem_Read(&hi2c3,HMC5883L_ADRESS,HMC5883l_ADD_DATAY_MSB_MULTI,1,DataY,2,100);
//		  Yaxis = ((DataY[1]<<8) | DataY[0])/ 660.f;
		  HAL_Delay(1000);
//		  lcd_clear ();
//		  HAL_Delay(50);
//		  lcd_put_cur(0, 0);

//		  sprintf(dataY, "Yd1: %d", Yaxis);
//		  lcd_put_cur(1, 0);
//		  lcd_send_string (dataY);
		  //HAL_UART_Transmit(&huart2, &dataY[], 10, 1000);
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);



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

static void MX_I2C3_Init(void)
{

  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c3);

}

static void MX_TIM4_Init(void)
{
	timer4.Instance = TIM4;
	timer4.Init.Period = 32000 - 1;
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

