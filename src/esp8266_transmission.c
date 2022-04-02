#include "esp8266_transmission.h"
#include <stdio.h>

extern CurrentATcommand currentAtCommand;
extern UART_HandleTypeDef huart2;

void initSendingCommands(SendingCommands *sendingCommands)
{
	//sprintf("AT\r\n", sendingCommands->AT);
	sendingCommands->AT[0] = 'A';
	sendingCommands->AT[1] = 'T';
	sendingCommands->AT[2] = '\r';
	sendingCommands->AT[3] = '\n';

	sendingCommands->responseAT[0] = '\r';
	sendingCommands->responseAT[1] = '\n';
	sendingCommands->responseAT[2] = 'O';
	sendingCommands->responseAT[3] = 'K';
	sendingCommands->responseAT[4] = '\r';
	sendingCommands->responseAT[5] = '\n';

	//AT+CWJAP="Nokia 8.3 5G",""
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
	sendingCommands->AT_CWJAP[10] = 'N';
	sendingCommands->AT_CWJAP[11] = 'o';
	sendingCommands->AT_CWJAP[12] = 'k';
	sendingCommands->AT_CWJAP[13] = 'i';
	sendingCommands->AT_CWJAP[14] = 'a';
	sendingCommands->AT_CWJAP[15] = ' ';
	sendingCommands->AT_CWJAP[16] = '8';
	sendingCommands->AT_CWJAP[17] = '.';
	sendingCommands->AT_CWJAP[18] = '3';
	sendingCommands->AT_CWJAP[19] = ' ';
	sendingCommands->AT_CWJAP[20] = '5';
	sendingCommands->AT_CWJAP[21] = 'G';
	sendingCommands->AT_CWJAP[22] = '"';
	sendingCommands->AT_CWJAP[23] = ',';
	sendingCommands->AT_CWJAP[24] = '"';
	sendingCommands->AT_CWJAP[25] = '"';
	sendingCommands->AT_CWJAP[26] = '\r';
	sendingCommands->AT_CWJAP[27] = '\n';

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

	//AT+CIPSTART="TCP","api.thingspeak.com",80
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
	sendingCommands->AT_CIPSTART[19] = 'a';
	sendingCommands->AT_CIPSTART[20] = 'p';
	sendingCommands->AT_CIPSTART[21] = 'i';
	sendingCommands->AT_CIPSTART[22] = '.';
	sendingCommands->AT_CIPSTART[23] = 't';
	sendingCommands->AT_CIPSTART[24] = 'h';
	sendingCommands->AT_CIPSTART[25] = 'i';
	sendingCommands->AT_CIPSTART[26] = 'n';
	sendingCommands->AT_CIPSTART[27] = 'g';
	sendingCommands->AT_CIPSTART[28] = 's';
	sendingCommands->AT_CIPSTART[29] = 'p';
	sendingCommands->AT_CIPSTART[30] = 'e';
	sendingCommands->AT_CIPSTART[31] = 'a';
	sendingCommands->AT_CIPSTART[32] = 'k';
	sendingCommands->AT_CIPSTART[33] = '.';
	sendingCommands->AT_CIPSTART[34] = 'c';
	sendingCommands->AT_CIPSTART[35] = 'o';
	sendingCommands->AT_CIPSTART[36] = 'm';
	sendingCommands->AT_CIPSTART[37] = '"';
	sendingCommands->AT_CIPSTART[38] = ',';
	sendingCommands->AT_CIPSTART[39] = '8';
	sendingCommands->AT_CIPSTART[40] = '0';
	sendingCommands->AT_CIPSTART[41] = '\r';
	sendingCommands->AT_CIPSTART[42] = '\n';

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
	sendingCommands->AT_CIPSEND[11] = '5';
	sendingCommands->AT_CIPSEND[12] = '1';
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

void sendDataToServer()
{

}

uint8_t receiveATresponse(SendingCommands *sendingCommands)
{
	HAL_UART_Receive_IT(&huart2, sendingCommands->responseAT, 6);
	if(HAL_UART_Transmit(&huart2, sendingCommands->AT_CIPSTART, sizeof(sendingCommands->AT_CIPSTART), 100) != HAL_OK)
	{
		char nok[] = "HAL_NOK AT!";
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
	    return 1;
	}
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
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
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
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
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
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
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
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
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
	    lcd_clear();
	    lcd_put_cur(0, 0);
	    lcd_send_string(nok);
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

