#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"

#include <string.h>
#include "stm32f4xx.h"
#include "lcd_display.h"
#include "magnetometer.h"
#include "accelerometer.h"
#include "gsm_transmission.h"
#include "esp8266_transmission.h"

UART_HandleTypeDef huart2, huart1, huart6;


I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef timer2, timer4;
ConnectionCommands connectionCommands;

SendingCommands sendingCommands;
volatile CurrentATcommand currentAtCommand = AT;
uint8_t sendWifiConnect = 0;
volatile int received=2;
uint8_t data[60] ="hello\r\n";
const uint8_t size = 10;
uint8_t countSendMsg = 0;

volatile uint8_t rec[10];

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART6_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart6);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	received=8;

	if(huart->Instance == USART1)
	{
		currentAtCommand = RESPONSE_CWJAP_RECEIVED;

	}
	else if(huart->Instance == USART2)
	{
		HAL_UART_Receive_IT(&huart2, &rec, 1);
	}
	else if(huart->Instance == USART6)
	{
		int i = 0;
		switch(currentAtCommand)
		{
		case AT:
			for( i=0; i<size; i++ )
			{
				if(rec[i] == 'O' || rec[i] == 'K')
				{
					currentAtCommand = RESPONSE_AT_RECEIVED;
					break;
				}
			}
			HAL_UART_Receive_IT(&huart6, &rec, size);
			break;
		case CWMODE:
			for( i=0; i<size; i++ )
			{
				if(rec[i] == 'O' || rec[i] == 'K')
				{
					currentAtCommand = RESPONSE_AT_CWMODE_RECEIVED;
					break;
				}
			}
			HAL_UART_Receive_IT(&huart6, &rec, size);
			break;
		case CWJAP:
			for( i=0; i<size; i++ )
			{
				if(rec[i] == 'W' || rec[i] == 'I')
				{
					currentAtCommand = RESPONSE_CWJAP_RECEIVED;
					break;
				}
			}
			HAL_UART_Receive_IT(&huart6, &rec, size);
			break;
		}

	}

}

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
		//the instance of timer2 is used to triger averaging maesured data
		if(accelerationDataReadingIndicator == READING_ACCELERATION)
		{
			accelerationDataReadingIndicator = AVERAGING_ACCELERATION;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
	}
	else if(htim->Instance == TIM4)
	{

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
	 __USART6_CLK_ENABLE();
	 __USART1_CLK_ENABLE();
	 __HAL_RCC_USART1_CLK_ENABLE();
	 __HAL_RCC_USART2_CLK_ENABLE();
	 __HAL_RCC_USART6_CLK_ENABLE();


	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOC_CLK_ENABLE();


    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7/*|GPIO_PIN_8*/;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);



	GPIO_InitTypeDef gpio_I2C1_SDA_SCL;
	gpio_I2C1_SDA_SCL.Pin = /*GPIO_PIN_4 |*/GPIO_PIN_8 | GPIO_PIN_9;
	gpio_I2C1_SDA_SCL.Mode = GPIO_MODE_AF_OD;
			// SCL, SDA
	gpio_I2C1_SDA_SCL.Pull = GPIO_NOPULL;
	gpio_I2C1_SDA_SCL.Alternate = GPIO_AF4_I2C1;
	gpio_I2C1_SDA_SCL.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_I2C1_SDA_SCL);

	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

	MX_I2C1_Init();
	lcdInit ();
//	MX_TIM4_Init();
	MX_TIM2_Init();
//    MX_USART2_UART_Init();
//    MX_USART1_UART_Init();
//    MX_USART6_UART_Init();

//    initConnectionCommands(&connectionCommands);
    initSendingCommands(&sendingCommands);
    initAccelerometer();

	initHMC5883L();
	lcdClear();

	waitTillAccelerometerIsInitialized();
	waitTillMagnetometerIsInitialized();

//	int counterOfTx=0;
//
//	HAL_UART_Receive_IT(&huart6, &rec, size);
//	HAL_UART_Receive_IT(&huart2, &rec, 1);
	int c = 0;
	while (1)
	{
		if(1 == checkAvalibilityOfDataInRegister())
		{
			XYZaxisAccelerationMS2 accel;
		    accel = getCalculatedAcceleration();
		    float degree;

		    if(AVERAGING_ACCELERATION == accelerationDataReadingIndicator && accel.validAcceleration == 1)
		    {
		     	degree = calculateAzimutWithDegree();
				char accelerationReadString[16];
			    char magnitudeReadString[16];
			    lcdClear();
			    lcdSetCursor(0, 0);
			    sprintf(magnitudeReadString, "Azimuth %d", (int)degree);
			    lcdSendString (magnitudeReadString);
			    lcdSetCursor(1, 0);
			    if(accel.xAcc.integerPart == 0 && accel.xAcc.floatingPart < 0)
			    {
			    	if(abs(accel.xAcc.floatingPart)  < 10)
			    	{
			    		sprintf(accelerationReadString, "Accel_x -%d.00%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    	else if(abs(accel.xAcc.floatingPart)  >= 10 && abs(accel.xAcc.floatingPart) < 100)
			    	{
			    		sprintf(accelerationReadString, "Accel_x -%d.0%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    	else
			    	{
			    		sprintf(accelerationReadString, "Accel_x -%d.%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    }
			    else if(accel.xAcc.integerPart < 0 && accel.xAcc.floatingPart < 0)
			    {
			    	if(abs(accel.xAcc.floatingPart)  < 10)
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.00%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    	else if(abs(accel.xAcc.floatingPart)  >= 10 && abs(accel.xAcc.floatingPart) < 100)
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.0%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    	else
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.%d", accel.xAcc.integerPart, (-1) * accel.xAcc.floatingPart);
			    	}
			    }
			    else
			    {
			    	if(abs(accel.xAcc.floatingPart)  < 10)
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.00%d", accel.xAcc.integerPart, accel.xAcc.floatingPart);
			    	}
			    	else if(abs(accel.xAcc.floatingPart)  >= 10 && abs(accel.xAcc.floatingPart) < 100)
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.0%d", accel.xAcc.integerPart, accel.xAcc.floatingPart);
			    	}
			    	else
			    	{
			    		sprintf(accelerationReadString, "Accel_x %d.%d", accel.xAcc.integerPart,  accel.xAcc.floatingPart);
			    	}
			    }

//			    sprintf(accelerationReadString, "Y %d.%d -%s1", accel.yAcc.integerPart, accel.yAcc.floatingPart, &connectionCommands.AT);

//			    sprintf(accelerationReadString, "Z2 %d", accel.xScaledAcc);

			    lcdSendString(accelerationReadString);

				char measurements[70];
				char frameToSend[70];
				sprintf(frameToSend, "%d %d %d %d %d \r\n", (int)degree,
						 accel.xScaledAcc,
						 accel.yScaledAcc,
						 accel.zScaledAcc,
						 50);
				sprintf(measurements, "Deg %d, Xacc: %d, Yacc: %d, Zacc: %d rec:%d atsize:%d\r\n", (int)degree, accel.xScaledAcc,
						 accel.yScaledAcc,
						 accel.zScaledAcc, received, strlen(sendingCommands.AT));
//				if(HAL_UART_Transmit(&huart2, measurements, strlen(measurements), 120) != HAL_OK)
//				{
//					char nok[] = "HAL_NOK meas!";
//					lcdClear();
//					lcdSetCursor(0, 0);
//					lcdSendString(nok);
//				    HAL_Delay(5000);
//				    //return 1;
//				}
//				HAL_UART_Transmit(&huart2, frameToSend, strlen(frameToSend), 100);
//
//
//				if(countSendMsg <= 4)
//				{
//					sendAT_CIPSTART();
//					HAL_Delay(50);
//					sendAT_CIPSEND(strlen(frameToSend));
//					HAL_Delay(50);
//					sendMessage(frameToSend);
//					HAL_Delay(50);
//					sendAT_CIPCLOSE();
//					sendWifiConnect = 1;
//				}
//				else
//				{
//					sendAT();
//				}
//				HAL_UART_Transmit(&huart2, sendingCommands.AT_CIPSEND, strlen(sendingCommands.AT_CIPSEND), 100);
//				countSendMsg++;
//				char st[6];
//				sprintf(st, "Sta:%d", currentAtCommand);
//			    lcdClear();
//			    lcdSetCursor(0, 0);
//			    lcdSendString(st);
//			    HAL_Delay(800);
//
			    accelerationDataReadingIndicator = READING_ACCELERATION;
			}
		}
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
	const uint16_t durationBetweenSendingTwoMeasurementsInMs = 1000;

	timer2.Instance = TIM2;
	timer2.Init.Period = 2 * durationBetweenSendingTwoMeasurementsInMs - 1;
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
	const uint16_t periodInMs = 100;

	timer4.Instance = TIM4;
	timer4.Init.Period = 2 * periodInMs - 1;
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
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);

	__HAL_UART_ENABLE_IT(&huart2,  UART_IT_RXNE);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
      while(1);
    }

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
}

void MX_USART6_UART_Init(void)
{
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 115200;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX ;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_Delay(200);
    if (HAL_UART_Init(&huart6) != HAL_OK)
    {
    	while(1)
    	{

    	}
    }
	__HAL_UART_ENABLE_IT(&huart6,  UART_IT_RXNE);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
}

