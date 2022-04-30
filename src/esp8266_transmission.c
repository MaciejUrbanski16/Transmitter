#include "esp8266_transmission.h"
#include <stdio.h>

extern CurrentATcommand currentAtCommand;
extern SendingCommands sendingCommands;
extern UART_HandleTypeDef huart2, huart1;
extern volatile uint8_t rec[10];

void initSendingCommands(SendingCommands *sendingCommands)
{
	//sprintf("AT\r\n", sendingCommands->AT);
//	sendingCommands->AT[0] = '\r';
//	sendingCommands->AT[1] = '\n';
	sendingCommands->AT[0] = 'A';
	sendingCommands->AT[1] = 'T';
	sendingCommands->AT[2] = '\r';
	sendingCommands->AT[3] = '\n';
//	sendingCommands->AT[4] = '\0';

	sendingCommands->responseAT[0] = '\r';
	sendingCommands->responseAT[1] = '\n';
	sendingCommands->responseAT[2] = 'O';
	sendingCommands->responseAT[3] = 'K';
	sendingCommands->responseAT[4] = '\r';
	sendingCommands->responseAT[5] = '\n';

	sendingCommands->AT_CWMODE[0] = 'A';
	sendingCommands->AT_CWMODE[1] = 'T';
	sendingCommands->AT_CWMODE[2] = '+';
	sendingCommands->AT_CWMODE[3] = 'C';
	sendingCommands->AT_CWMODE[4] = 'W';
	sendingCommands->AT_CWMODE[5] = 'M';
	sendingCommands->AT_CWMODE[6] = 'O';
	sendingCommands->AT_CWMODE[7] = 'D';
	sendingCommands->AT_CWMODE[8] = 'E';
	sendingCommands->AT_CWMODE[9] = '=';
	sendingCommands->AT_CWMODE[10] = '3';
	sendingCommands->AT_CWMODE[11] = '\r';
	sendingCommands->AT_CWMODE[12] = '\n';

	//AT+CWJAP="InfaNET-K-1234","qwertyuiop"
	sendingCommands->AT_CWJAP[0] = 'A';
	sendingCommands->AT_CWJAP[1] = 'T';
	sendingCommands->AT_CWJAP[2] = '+';
	sendingCommands->AT_CWJAP[3] = 'C';
	sendingCommands->AT_CWJAP[4] = 'W';
	sendingCommands->AT_CWJAP[5] = 'J';
	sendingCommands->AT_CWJAP[6] = 'A';
	sendingCommands->AT_CWJAP[7] = 'P';
	sendingCommands->AT_CWJAP[8] = '=';
	sendingCommands->AT_CWJAP[9] = '"';
	sendingCommands->AT_CWJAP[10] = 'I';
	sendingCommands->AT_CWJAP[11] = 'n';
	sendingCommands->AT_CWJAP[12] = 'f';
	sendingCommands->AT_CWJAP[13] = 'a';
	sendingCommands->AT_CWJAP[14] = 'N';
	sendingCommands->AT_CWJAP[15] = 'E';
	sendingCommands->AT_CWJAP[16] = 'T';
	sendingCommands->AT_CWJAP[17] = '-';
	sendingCommands->AT_CWJAP[18] = 'K';
	sendingCommands->AT_CWJAP[19] = '-';
	sendingCommands->AT_CWJAP[20] = '1';
	sendingCommands->AT_CWJAP[21] = '2';
	sendingCommands->AT_CWJAP[22] = '3';
	sendingCommands->AT_CWJAP[23] = '4';
	sendingCommands->AT_CWJAP[24] = '"';
	sendingCommands->AT_CWJAP[25] = ',';
	sendingCommands->AT_CWJAP[26] = '"';
	sendingCommands->AT_CWJAP[27] = 'q';
	sendingCommands->AT_CWJAP[28] = 'w';
	sendingCommands->AT_CWJAP[29] = 'e';
	sendingCommands->AT_CWJAP[30] = 'r';
	sendingCommands->AT_CWJAP[31] = 't';
	sendingCommands->AT_CWJAP[32] = 'y';
	sendingCommands->AT_CWJAP[33] = 'u';
	sendingCommands->AT_CWJAP[34] = 'i';
	sendingCommands->AT_CWJAP[35] = 'o';
	sendingCommands->AT_CWJAP[36] = 'p';
	sendingCommands->AT_CWJAP[37] = '"';
	sendingCommands->AT_CWJAP[38] = '\r';
	sendingCommands->AT_CWJAP[39] = '\n';

	//AT+CIPMUX=0
	sendingCommands->AT_CIPMUX[0] = 'A';
	sendingCommands->AT_CIPMUX[1] = 'T';
	sendingCommands->AT_CIPMUX[2] = '+';
	sendingCommands->AT_CIPMUX[3] = 'C';
	sendingCommands->AT_CIPMUX[4] = 'I';
	sendingCommands->AT_CIPMUX[5] = 'P';
	sendingCommands->AT_CIPMUX[6] = 'M';
	sendingCommands->AT_CIPMUX[7] = 'U';
	sendingCommands->AT_CIPMUX[8] = 'X';
	sendingCommands->AT_CIPMUX[9] = '=';
	sendingCommands->AT_CIPMUX[10] = '0';
	sendingCommands->AT_CIPMUX[11] = '\r';
	sendingCommands->AT_CIPMUX[12] = '\n';

	//AT+CIPSTART="TCP","10.0.0.100",8000
	sendingCommands->AT_CIPSTART[0] = 'A';
	sendingCommands->AT_CIPSTART[1] = 'T';
	sendingCommands->AT_CIPSTART[2] = '+';
	sendingCommands->AT_CIPSTART[3] = 'C';
	sendingCommands->AT_CIPSTART[4] = 'I';
	sendingCommands->AT_CIPSTART[5] = 'P';
	sendingCommands->AT_CIPSTART[6] = 'S';
	sendingCommands->AT_CIPSTART[7] = 'T';
	sendingCommands->AT_CIPSTART[8] = 'A';
	sendingCommands->AT_CIPSTART[9] = 'R';
	sendingCommands->AT_CIPSTART[10] = 'T';
	sendingCommands->AT_CIPSTART[11] = '=';
	sendingCommands->AT_CIPSTART[12] = '"';
	sendingCommands->AT_CIPSTART[13] = 'T';
	sendingCommands->AT_CIPSTART[14] = 'C';
	sendingCommands->AT_CIPSTART[15] = 'P';
	sendingCommands->AT_CIPSTART[16] = '"';
	sendingCommands->AT_CIPSTART[17] = ',';
	sendingCommands->AT_CIPSTART[18] = '"';
	sendingCommands->AT_CIPSTART[19] = '1';
	sendingCommands->AT_CIPSTART[20] = '0';
	sendingCommands->AT_CIPSTART[21] = '.';
	sendingCommands->AT_CIPSTART[22] = '0';
	sendingCommands->AT_CIPSTART[23] = '.';
	sendingCommands->AT_CIPSTART[24] = '0';
	sendingCommands->AT_CIPSTART[25] = '.';
	sendingCommands->AT_CIPSTART[26] = '1';
	sendingCommands->AT_CIPSTART[27] = '0';
	sendingCommands->AT_CIPSTART[28] = '0';
	sendingCommands->AT_CIPSTART[29] = '"';
	sendingCommands->AT_CIPSTART[30] = ',';
	sendingCommands->AT_CIPSTART[31] = '8';
	sendingCommands->AT_CIPSTART[32] = '0';
	sendingCommands->AT_CIPSTART[33] = '0';
	sendingCommands->AT_CIPSTART[34] = '0';
	sendingCommands->AT_CIPSTART[35] = '\r';
	sendingCommands->AT_CIPSTART[36] = '\n';

	//AT+CIPSEND=51
	sendingCommands->AT_CIPSEND[0] = 'A';
	sendingCommands->AT_CIPSEND[1] = 'T';
	sendingCommands->AT_CIPSEND[2] = '+';
	sendingCommands->AT_CIPSEND[3] = 'C';
	sendingCommands->AT_CIPSEND[4] = 'I';
	sendingCommands->AT_CIPSEND[5] = 'P';
	sendingCommands->AT_CIPSEND[6] = 'S';
	sendingCommands->AT_CIPSEND[7] = 'E';
	sendingCommands->AT_CIPSEND[8] = 'N';
	sendingCommands->AT_CIPSEND[9] = 'D';
	sendingCommands->AT_CIPSEND[10] = '=';
	sendingCommands->AT_CIPSEND[11] = '1';
	sendingCommands->AT_CIPSEND[12] = '0';
	sendingCommands->AT_CIPSEND[13] = '\r';
	sendingCommands->AT_CIPSEND[14] = '\n';

	//AT+CIPCLOSE
	sendingCommands->AT_CIPCLOSE[0] = 'A';
	sendingCommands->AT_CIPCLOSE[1] = 'T';
	sendingCommands->AT_CIPCLOSE[2] = '+';
	sendingCommands->AT_CIPCLOSE[3] = 'C';
	sendingCommands->AT_CIPCLOSE[4] = 'I';
	sendingCommands->AT_CIPCLOSE[5] = 'P';
	sendingCommands->AT_CIPCLOSE[6] = 'C';
	sendingCommands->AT_CIPCLOSE[7] = 'L';
	sendingCommands->AT_CIPCLOSE[8] = 'O';
	sendingCommands->AT_CIPCLOSE[9] = 'S';
	sendingCommands->AT_CIPCLOSE[10] = 'E';
	sendingCommands->AT_CIPCLOSE[11] = '\r';
	sendingCommands->AT_CIPCLOSE[12] = '\n';

}


void debug(int state)
{
	char stateToPrint[20];
	sprintf(stateToPrint, "Current state: %d\r\n", state);
	if(HAL_UART_Transmit(&huart2, stateToPrint, sizeof(stateToPrint), 50) != HAL_OK)
	{
		char nok[20];
		sprintf(nok, "HAL_NOK ST: %d\r\n", state);
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    //return 1;
	}
}

void sendAT()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT, strlen(sendingCommands.AT), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT, strlen(sendingCommands.AT), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = AT;
		return;
	}
	uint8_t errorLog[] = "AT was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendAT_CWMODE()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT_CWMODE, strlen(sendingCommands.AT_CWMODE), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT_CWMODE, strlen(sendingCommands.AT_CWMODE), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CWMODE;
		return;
	}
	uint8_t errorLog[] = "AT+CWMODE was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendAT_CWJAP()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT_CWJAP, strlen(sendingCommands.AT_CWJAP), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT_CWJAP, strlen(sendingCommands.AT_CWJAP), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CWJAP;
		return;
	}
	uint8_t errorLog[] = "AT+CWJAP was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendAT_CIPSTART()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT_CIPSTART, strlen(sendingCommands.AT_CIPSTART), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT_CIPSTART, strlen(sendingCommands.AT_CIPSTART), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CIPSTART;
		return;
	}
	uint8_t errorLog[] = "AT_CIPSTART was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendAT_CIPSEND()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT_CIPSEND, strlen(sendingCommands.AT_CIPSEND), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT_CIPSEND, strlen(sendingCommands.AT_CIPSEND), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CIPSEND;
		return;
	}
	uint8_t errorLog[] = "AT_CIPSEND was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendAT_CIPCLOSE()
{
	if(HAL_UART_Transmit(&huart1, sendingCommands.AT_CIPCLOSE, strlen(sendingCommands.AT_CIPCLOSE), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, sendingCommands.AT_CIPCLOSE, strlen(sendingCommands.AT_CIPCLOSE), 100);
		HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CIPCLOSE;
		return;
	}
	uint8_t errorLog[] = "AT_CIPCLOSE was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

void sendMessage()
{
	uint8_t msg[] = "12 2 4 2 2 \r\n";
	if(HAL_UART_Transmit(&huart1, msg, strlen(msg), 100) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, msg, strlen(msg), 100);
	//	HAL_UART_Transmit(&huart2, rec, strlen(rec), 100);
		currentAtCommand = CIPCLOSE;
		return;
	}
	uint8_t errorLog[] = "msg was not sent\r\n";
	HAL_UART_Transmit(&huart2, errorLog, strlen(errorLog), 100);
}

////////////////////////
uint8_t receiveATresponse(SendingCommands *sendingCommands)
{

	//debug(currentAtCommand);

	if(HAL_UART_Transmit(&huart2, sendingCommands->AT, sizeof(sendingCommands->AT), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK AT!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT, 6);
	if(currentAtCommand == RESPONSE_AT_RECEIVED)
	{
		if(checkResponseATCorrectness(sendingCommands))
		{
			currentAtCommand = CWJAP;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseATCorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

uint8_t receiveCWJAPresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT_CWJAP, 6);//6??
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CWJAP, sizeof(sendingCommands->AT_CWJAP), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK CWJAP!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	if(currentAtCommand == RESPONSE_CWJAP_RECEIVED)
	{
		if(checkResponseCWJAPCorrectness(sendingCommands))
		{
			currentAtCommand = CIPMUX;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseCWJAPCorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

uint8_t receiveCIPMUXresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT_CIPMUX, 6);//6??
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CIPMUX, sizeof(sendingCommands->AT_CIPMUX), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK CIPMUX!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	if(currentAtCommand == RESPONSE_CIPMUX_RECEIVED)
	{
		if(checkResponseCIPMUXCorrectness(sendingCommands))
		{
			currentAtCommand = CIPSTART;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseCIPMUXCorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

uint8_t receiveCIPSTARTresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT_CIPSTART, 6);//6??
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CIPSTART, sizeof(sendingCommands->AT_CIPSTART), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK CIPSTART!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	if(currentAtCommand == RESPONSE_CIPSTART_RECEIVED)
	{
		if(checkResponseCIPSTARTCorrectness(sendingCommands))
		{
			currentAtCommand = CIPSEND;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseCIPSTARTCorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

uint8_t receiveCIPSENDresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT_CIPSEND, 6);//6??
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CIPSEND, sizeof(sendingCommands->AT_CIPSEND), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK when CIPSEND!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	if(currentAtCommand == RESPONSE_CIPSEND_RECEIVED)
	{
		if(checkResponseCIPSENDCorrectness(sendingCommands))
		{
			currentAtCommand = SEND_DATA;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseCIPSENDCorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

uint8_t receiveCIPCLOSEresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT_CIPCLOSE, 6);//6??
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CIPCLOSE, sizeof(sendingCommands->AT_CIPCLOSE), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK when CIPCLOSE!";
	    lcdClear();
	    lcdSetCursor(0, 0);
	    lcdSendString(nok);
	    HAL_Delay(2000);
	    return 1;
	}
	if(currentAtCommand == RESPONSE_CIPCLOSE_RECEIVED)
	{
		if(checkResponseCIPCLOSECorrectness(sendingCommands))
		{
			currentAtCommand = AT;
			return 0;
		}
	}
	return 1;
}

uint8_t checkResponseCIPCLOSECorrectness(SendingCommands *sendingCommands)
{
	return 1;
}

