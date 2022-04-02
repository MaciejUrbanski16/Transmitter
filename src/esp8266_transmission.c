#include "esp8266_transmission.h"
#include <stdio.h>


void initSendingCommands(SendingCommands *sendingCommands)
{
	//sprintf("AT\r\n", sendingCommands->AT);
	sendingCommands->AT[0] = 'A';
	sendingCommands->AT[1] = 'T';
	sendingCommands->AT[2] = '\r';
	sendingCommands->AT[3] = '\n';

	sendingCommands->responseAT[0] = 'O';
	sendingCommands->responseAT[0] = 'K';
}
