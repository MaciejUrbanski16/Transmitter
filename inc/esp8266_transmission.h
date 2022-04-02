/*
 * esp8266_transmission.h
 *
 *  Created on: 01.04.2022
 *      Author: Admin
 */

#ifndef INC_ESP8266_TRANSMISSION_H_
#define INC_ESP8266_TRANSMISSION_H_

#include "stm32f4xx_hal.h"

typedef enum
{
	AT =0,
	CWJAP,
	CIPMUX,
	CIPSTART,
	CIPSEND,
	CIPCLOSE
}CurrentATcommand;

typedef struct
{
	uint8_t AT[4];
	uint8_t responseAT[2];

	char AT_CWJAP[26]; //26?			//AT+CWJAP="Nokia 8.3 5G",""
	char responseAT_CWJAP[14];		//WIFI CONNECTED

	char AT_CIPMUX[11];				//AT+CIPMUX=0
	char responseAT_CIPMUX[2];

	char AT_CIPSTART[39];//39?		//AT+CIPSTART="TCP","api.thingspeak.com",80
	char responseAT_CIPSTART[7];		//CONNECT

	char AT_CIPSEND[13];				//AT+CIPSEND=51
	char responseAT_CIPSEND[2];		//OK

	char updateFieldOnServer[100];	//GET /update?api_key=XY42MIVU2SXRRZVL&field1=215

	char AT_CIPCLOSE[11];			//AT+CIPCLOSE
}SendingCommands;

void initSendingCommands(SendingCommands *sendingCommands);

#endif /* INC_ESP8266_TRANSMISSION_H_ */
