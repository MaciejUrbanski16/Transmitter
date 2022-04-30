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
	AT = 0,
	RESPONSE_AT_RECEIVED = 1,
	CWJAP = 2,
	RESPONSE_CWJAP_RECEIVED = 3,
	CIPMUX = 4,
	RESPONSE_CIPMUX_RECEIVED = 5,
	CIPSTART = 6,
	RESPONSE_CIPSTART_RECEIVED = 7,
	CIPSEND = 8,
	RESPONSE_CIPSEND_RECEIVED = 9,
	SEND_DATA = 10,
	CIPCLOSE = 11,
	RESPONSE_CIPCLOSE_RECEIVED = 12
}CurrentATcommand;

typedef struct
{
	uint8_t AT[12];
	uint8_t responseAT[2];

	uint8_t AT_CWJAP[28]; //26?			//AT+CWJAP="Nokia 8.3 5G",""
	uint8_t responseAT_CWJAP[14];		//WIFI CONNECTED

	uint8_t AT_CIPMUX[13];				//AT+CIPMUX=0
	uint8_t responseAT_CIPMUX[2];

	uint8_t AT_CIPSTART[43];//39?		//AT+CIPSTART="TCP","api.thingspeak.com",80
	uint8_t responseAT_CIPSTART[7];		//CONNECT

	uint8_t AT_CIPSEND[15];				//AT+CIPSEND=51
	uint8_t responseAT_CIPSEND[2];		//OK

	uint8_t updateFieldOnServer[100];	//GET /update?api_key=XY42MIVU2SXRRZVL&field1=215

	uint8_t AT_CIPCLOSE[13];			//AT+CIPCLOSE
	uint8_t responseAT_CIPCLOSE[8];
}SendingCommands;

void debug(int state);

void initSendingCommands(SendingCommands *sendingCommands);


uint8_t receiveATresponse(SendingCommands *sendingCommands);
uint8_t checkResponseATCorrectness(SendingCommands *sendingCommands);

uint8_t receiveCWJAPresponse(SendingCommands *sendingCommands);
uint8_t checkResponseCWJAPCorrectness(SendingCommands *sendingCommands);

uint8_t receiveCIPMUXresponse(SendingCommands *sendingCommands);
uint8_t checkResponseCIPMUXCorrectness(SendingCommands *sendingCommands);

uint8_t receiveCIPSTARTresponse(SendingCommands *sendingCommands);
uint8_t checkResponseCIPSTARTCorrectness(SendingCommands *sendingCommands);

uint8_t receiveCIPSENDresponse(SendingCommands *sendingCommands);
uint8_t checkResponseCIPSENDCorrectness(SendingCommands *sendingCommands);

uint8_t receiveCIPCLOSEresponse(SendingCommands *sendingCommands);
uint8_t checkResponseCIPCLOSECorrectness(SendingCommands *sendingCommands);

#endif /* INC_ESP8266_TRANSMISSION_H_ */
