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
#include "accelerometer.h"
#include "gsm_transmission.h"
#include "esp8266_transmission.h"

UART_HandleTypeDef huart2, huart1;

I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef timer2, timer4;
ConnectionCommands connectionCommands;

SendingCommands sendingCommands;
CurrentATcommand currentAtCommand = AT;
uint8_t data[60] ="hello\r\n";

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		switch(currentAtCommand)
		{
		case AT:
			currentAtCommand = RESPONSE_AT_RECEIVED;
			break;
		case CWJAP:
			currentAtCommand = RESPONSE_CWJAP_RECEIVED;
			break;
		case CIPMUX:
			currentAtCommand = RESPONSE_CIPMUX_RECEIVED;
			break;
		case CIPSTART:
			currentAtCommand = RESPONSE_CIPSTART_RECEIVED;
			break;
		case CIPSEND:
			currentAtCommand = RESPONSE_CIPSEND_RECEIVED;
			break;
//		case SEND_DATA:
//			sendDataToServer();
//			currentAtCommand = CIPCLOSE;
		case CIPCLOSE:
			currentAtCommand = RESPONSE_CIPCLOSE_RECEIVED;
			break;
		}
	}
}

void sendDataToServer(float azimuth, XYZaxisAccelerationMS2 accel)
{
	char rcvd_data[10];
	HAL_UART_Receive_IT(&huart2, &rcvd_data, 10);
	switch (currentAtCommand)
	{
	case RESPONSE_AT_RECEIVED:
		currentAtCommand = CWJAP;
		break;
	case RESPONSE_CWJAP_RECEIVED:
		currentAtCommand = CIPMUX;
		break;
	case RESPONSE_CIPMUX_RECEIVED:
		currentAtCommand = CIPSTART;
		break;
	case RESPONSE_CIPSTART_RECEIVED:
		currentAtCommand = CIPSEND;
		break;
	case RESPONSE_CIPSEND_RECEIVED:
		//sending...
		currentAtCommand = CIPSTART;
		break;
	}


}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
//		char stateToPrint[30];
//		sprintf(stateToPrint, "TxCallba state: %d\r\n", currentAtCommand);
//		if(HAL_UART_Transmit(&huart2, stateToPrint, sizeof(stateToPrint), 80) != HAL_OK)
//		{
//			char nok[20];
//			sprintf(nok, "HAL_NOK ST: %d\r\n", currentAtCommand);
//		    lcd_clear();
//		    lcd_put_cur(0, 0);
//		    lcd_send_string(nok);
//		    HAL_Delay(5000);
//		    //return 1;
//		}

		switch(currentAtCommand)
		{
		case RESPONSE_AT_RECEIVED:
			currentAtCommand = CWJAP;
			break;
		case RESPONSE_CWJAP_RECEIVED:
			currentAtCommand = CIPMUX;
			break;
		case RESPONSE_CIPMUX_RECEIVED:
			currentAtCommand = CIPSTART;
			break;
		case RESPONSE_CIPSTART_RECEIVED:
			currentAtCommand = CIPSEND;
			break;
		case RESPONSE_CIPSEND_RECEIVED:
			//currentAtCommand = RESPONSE_CIPSEND_RECEIVED;
			break;
//		case SEND_DATA:
//			sendDataToServer();
//			currentAtCommand = CIPCLOSE;
		case RESPONSE_CIPCLOSE_RECEIVED:
			currentAtCommand = AT;
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
	 __USART1_CLK_ENABLE();

	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7/*|GPIO_PIN_8*/;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
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
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

	MX_I2C1_Init();
	MX_TIM4_Init();
	MX_TIM2_Init();
    MX_USART2_UART_Init();
    MX_USART1_UART_Init();

    initConnectionCommands(&connectionCommands);
    initSendingCommands(&sendingCommands);
    initAccelerometer();
	lcd_init ();
	initHMC5883L();
	lcd_clear();


	waitTillAccelerometerIsInitialized();
	waitTillMagnetometerIsInitialized();

	int counterOfTx=0;

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
			    lcd_clear();
			    lcd_put_cur(0, 0);
			    sprintf(magnitudeReadString, "Degree %d", (int)degree);
			    lcd_send_string (magnitudeReadString);
			    lcd_put_cur(1, 0);
//			    sprintf(accelerationReadString, "X %d.%d", accel.xAcc.integerPart, accel.xAcc.floatingPart);
//			    sprintf(accelerationReadString, "Y %d.%d -%s1", accel.yAcc.integerPart, accel.yAcc.floatingPart, &connectionCommands.AT);
			    sprintf(accelerationReadString, "Z2 %d.%d", accel.zAcc.integerPart, accel.zAcc.floatingPart);
			    lcd_send_string(accelerationReadString);

				char measurements[70];
				sprintf(measurements, "Deg %d, Xacc: %d.%d, Yacc: %d.%d, Zacc: %d.%d \r\n", (int)degree, accel.xAcc.integerPart, accel.xAcc.floatingPart,
						accel.yAcc.integerPart, accel.yAcc.floatingPart, accel.zAcc.integerPart, accel.zAcc.floatingPart);
//				if(HAL_UART_Transmit(&huart2, measurements, sizeof(measurements), 120) != HAL_OK)
//				{
//					char nok[] = "HAL_NOK meas!";
//				    lcd_clear();
//				    lcd_put_cur(0, 0);
//				    lcd_send_string(nok);
//				    HAL_Delay(5000);
//				    //return 1;
//				}
				if(HAL_UART_Transmit(&huart2, sendingCommands.AT, sizeof(sendingCommands.AT), 100) != HAL_OK)
				{
					char nok[] = "HAL_NOK AT!";
				    lcd_clear();
				    lcd_put_cur(0, 0);
				    lcd_send_string(nok);
				    HAL_Delay(2000);
				    return 1;
				}
				char rec[6];
				if(HAL_UART_Receive_IT(&huart2, rec, 6) != HAL_OK)
				{
					char nok[] = "HAL_NOK ATR!";
				    lcd_clear();
				    lcd_put_cur(0, 0);
				    lcd_send_string(nok);
				    HAL_Delay(2000);
				   // return 1;
				}
				char st[6];
				sprintf(st, "Sta:%d", currentAtCommand);
			    lcd_clear();
			    lcd_put_cur(0, 0);
			    lcd_send_string(st);
			    HAL_Delay(2000);

				sendDataToServer(degree, accel);

			    accelerationDataReadingIndicator = READING_ACCELERATION;
			}
		}
		//HAL_UART_Receive_IT(&huart2, &rcvd_data,1);


//		switch(currentAtCommand)
//		{
//		case AT:
//			if(1 == receiveATresponse(&sendingCommands))
//			{
//
//			}
//			break;
//		case CWJAP:
//			if(1 == receiveCWJAPresponse(&sendingCommands))
//			{
//
//			}
//			break;
//		case CIPMUX:
//			if(1 == receiveCIPMUXresponse(&sendingCommands))
//			{
//
//			}
//			break;
//		case CIPSTART:
//			if(1 == receiveCIPSTARTresponse(&sendingCommands))
//			{
//
//			}
//			break;
//		case SEND_DATA:
//			sendDataToServer();
//			break;
//		default:
//			break;
//		}
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
	const uint16_t periodInMs = 1000;

	timer2.Instance = TIM2;
	timer2.Init.Period = 2 * periodInMs - 1;
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
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

void MX_USART1_UART_Init(void)
{
    huart2.Instance = USART1;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
}
