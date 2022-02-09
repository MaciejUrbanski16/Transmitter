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
		  counterTimer4++;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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
	__HAL_RCC_I2C1_CLK_ENABLE();

	__HAL_RCC_TIM4_CLK_ENABLE();
	//__HAL_RCC_I2C3_CLK_ENABLE();
	MX_I2C1_Init();
	//MX_I2C3_Init();
	MX_TIM4_Init();



    MX_USART2_UART_Init();





	 initHMC5883L();
	  lcd_init ();
	  lcd_clear();


	  char dataX[16];
	  char dataY[16];
	  //while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)HMC5883L_ADRESS, 10, 100) != HAL_OK)
	  {
		  lcd_clear();
		  lcd_put_cur(0, 0);
		  lcd_send_string ("I2C8tw. HMC");
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//		  HAL_Delay(1000);
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	  }

#define HMC5883l_ADD_DATAX_MSB_MULTI (XaxisMSB | 0x80)
#define HMC5883l_ADD_DATAY_MSB_MULTI (YaxisMSB | 0x80)
	  uint8_t DataX[2];
	  uint8_t DataY[2];
	  uint16_t Xaxis = 0;
	  uint16_t Yaxis = 0;


	  while (1)
	  {
		  HAL_I2C_Mem_Read(&hi2c1,HMC5883L_ADRESS,HMC5883l_ADD_DATAX_MSB_MULTI,1,DataX,2,100);
		  Xaxis = ((DataX[1]<<8) | DataX[0]);
		  HAL_I2C_Mem_Read(&hi2c1,HMC5883L_ADRESS,HMC5883l_ADD_DATAY_MSB_MULTI,1,DataY,2,100);
		  Yaxis = ((DataY[1]<<8) | DataY[0]);
		  HAL_Delay(1000);
		  lcd_clear ();
		  HAL_Delay(50);
		  lcd_put_cur(0, 0);
		  HAL_Delay(50);
		  sprintf(dataX, "Xd1: %d", Xaxis);
		  lcd_send_string (dataX);
		  sprintf(dataY, "Yd: %d", Yaxis);
		  lcd_put_cur(1, 0);
		  lcd_send_string (dataY);
		  //HAL_UART_Transmit(&huart2, &dataY[], 10, 1000);
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

static void MX_I2C3_Init(void)
{

  hi2c1.Instance = I2C3;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c3);

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

