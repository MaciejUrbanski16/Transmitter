/*
 * gsm_transmission.h
 *
 *  Created on: 19.02.2022
 *      Author: Admin
 */

#ifndef INC_GSM_TRANSMISSION_H_
#define INC_GSM_TRANSMISSION_H_

typedef struct
{
	char AT[4];
	char AT_CIPSHUT[10];
	char AT_CIPMODE[14];
	char AT_CIPMUX_0[13];
	char AT_CGATT_1[12];    // attach to packet doamin service

	char AT_CSTT_[19];      // set apn, username, password for the PDP context
	char AT_CIICR[10];      // bring up gsm call
	char AT_CIFSR[10];      // get local ip address

	char AT_CIPSTART[40];   // THIS CAN BE VARIOUS //init of connection with ip and port AT+CIPSTART="TCP","IP server adres","PORT"
	char AT_CIPSEND[12];    // send message in particualr port to particualr ip
	char dataToSend[100];   // THIS CAN BE VARIOUS
	char AT_CIPCLOSE[11];   // closing connection
	int sendCommand;        // 0x1A

	char OK[2]; 		    // OK response after command
	char SEND_OK[7];        // OK response from network after send
}ConnectionCommands;

void initConnectionCommands(ConnectionCommands *connectionCommands);

#endif /* INC_GSM_TRANSMISSION_H_ */
