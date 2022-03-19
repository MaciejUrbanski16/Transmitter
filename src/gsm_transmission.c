/*
 * gsm_transmission.c
 *
 *  Created on: 19.02.2022
 *      Author: Admin
 */

#include "gsm_transmission.h"

void initConnectionCommands(ConnectionCommands *connectionCommands)
{
	connectionCommands->AT[0] = 'A';
	connectionCommands->AT[1] = 'T';
	connectionCommands->AT[2] = '\r';
	connectionCommands->AT[3] = '\n';
//	sprintf("AT+CIPMODE=0", connectionCommands->AT_CIPMODE);

	connectionCommands->AT_CIPMODE[0] = 'A';
	connectionCommands->AT_CIPMODE[1] = 'T';
	connectionCommands->AT_CIPMODE[2] = '+';
	connectionCommands->AT_CIPMODE[3] = 'C';
	connectionCommands->AT_CIPMODE[4] = 'I';
	connectionCommands->AT_CIPMODE[5] = 'P';
	connectionCommands->AT_CIPMODE[6] = 'M';
	connectionCommands->AT_CIPMODE[7] = 'O';
	connectionCommands->AT_CIPMODE[8] = 'D';
	connectionCommands->AT_CIPMODE[9] = 'E';
	connectionCommands->AT_CIPMODE[10] = '=';
	connectionCommands->AT_CIPMODE[11] = '0';
	connectionCommands->AT_CIPMODE[12] = '\r';
	connectionCommands->AT_CIPMODE[13] = '\n';

	connectionCommands->AT_CIPMUX_0[0] = 'A';
	connectionCommands->AT_CIPMUX_0[1] = 'T';
	connectionCommands->AT_CIPMUX_0[2] = '+';
	connectionCommands->AT_CIPMUX_0[3] = 'C';
	connectionCommands->AT_CIPMUX_0[4] = 'I';
	connectionCommands->AT_CIPMUX_0[5] = 'P';
	connectionCommands->AT_CIPMUX_0[6] = 'M';
	connectionCommands->AT_CIPMUX_0[7] = 'U';
	connectionCommands->AT_CIPMUX_0[8] = 'X';
	connectionCommands->AT_CIPMUX_0[9] = '=';
	connectionCommands->AT_CIPMUX_0[10] = '0';
	connectionCommands->AT_CIPMUX_0[11] = '\r';
	connectionCommands->AT_CIPMUX_0[12] = '\n';

	connectionCommands->AT_CGATT_1[0] = 'A';
	connectionCommands->AT_CGATT_1[1] = 'T';
	connectionCommands->AT_CGATT_1[2] = '+';
	connectionCommands->AT_CGATT_1[3] = 'C';
	connectionCommands->AT_CGATT_1[4] = 'G';
	connectionCommands->AT_CGATT_1[5] = 'A';
	connectionCommands->AT_CGATT_1[6] = 'T';
	connectionCommands->AT_CGATT_1[7] = 'T';
	connectionCommands->AT_CGATT_1[8] = '=';
	connectionCommands->AT_CGATT_1[9] = '1';
	connectionCommands->AT_CGATT_1[10] = '\r';
	connectionCommands->AT_CGATT_1[11] = '\n';

	connectionCommands->AT_CSTT_[0] = 'A';
	connectionCommands->AT_CSTT_[1] = 'T';
	connectionCommands->AT_CSTT_[2] = '+';
	connectionCommands->AT_CSTT_[3] = 'C';
	connectionCommands->AT_CSTT_[4] = 'S';
	connectionCommands->AT_CSTT_[5] = 'T';
	connectionCommands->AT_CSTT_[6] = 'T';
	connectionCommands->AT_CSTT_[7] = '=';
	connectionCommands->AT_CSTT_[8] = '"';
	connectionCommands->AT_CSTT_[9] = 'i';
	connectionCommands->AT_CSTT_[10] = 'n';
	connectionCommands->AT_CSTT_[11] = 't';
	connectionCommands->AT_CSTT_[12] = 'e';
	connectionCommands->AT_CSTT_[13] = 'r';
	connectionCommands->AT_CSTT_[14] = 'n';
	connectionCommands->AT_CSTT_[15] = 'e';
	connectionCommands->AT_CSTT_[16] = 't';
	connectionCommands->AT_CSTT_[17] = '"';
	connectionCommands->AT_CSTT_[14] = ',';
	connectionCommands->AT_CSTT_[15] = '"';
	connectionCommands->AT_CSTT_[16] = '"';
	connectionCommands->AT_CSTT_[17] = ',';
	connectionCommands->AT_CSTT_[15] = '"';
	connectionCommands->AT_CSTT_[16] = '"';
	connectionCommands->AT_CSTT_[17] = '\r';
	connectionCommands->AT_CSTT_[18] = '\n';

	connectionCommands->AT_CIICR[0] = 'A';
	connectionCommands->AT_CIICR[1] = 'T';
	connectionCommands->AT_CIICR[2] = '+';
	connectionCommands->AT_CIICR[3] = 'C';
	connectionCommands->AT_CIICR[4] = 'I';
	connectionCommands->AT_CIICR[5] = 'I';
	connectionCommands->AT_CIICR[6] = 'C';
	connectionCommands->AT_CIICR[7] = 'R';
	connectionCommands->AT_CIICR[8] = '\r';
	connectionCommands->AT_CIICR[9] = '\n';

	connectionCommands->AT_CIFSR[0] = 'A';
	connectionCommands->AT_CIFSR[1] = 'T';
	connectionCommands->AT_CIFSR[2] = '+';
	connectionCommands->AT_CIFSR[3] = 'C';
	connectionCommands->AT_CIFSR[4] = 'I';
	connectionCommands->AT_CIFSR[5] = 'F';
	connectionCommands->AT_CIFSR[6] = 'S';
	connectionCommands->AT_CIFSR[7] = 'R';
	connectionCommands->AT_CIFSR[6] = '\r';
	connectionCommands->AT_CIFSR[7] = '\n';

	connectionCommands->AT_CIPSTART[0] = 'A';
	connectionCommands->AT_CIPSTART[1] = 'T';
	connectionCommands->AT_CIPSTART[2] = '+';
	connectionCommands->AT_CIPSTART[3] = 'C';
	connectionCommands->AT_CIPSTART[4] = 'I';
	connectionCommands->AT_CIPSTART[5] = 'P';
	connectionCommands->AT_CIPSTART[6] = 'S';
	connectionCommands->AT_CIPSTART[7] = 'T';
	connectionCommands->AT_CIPSTART[8] = 'A';
	connectionCommands->AT_CIPSTART[9] = 'R';
	connectionCommands->AT_CIPSTART[10] = 'T';
	connectionCommands->AT_CIPSTART[11] = '=';
	connectionCommands->AT_CIPSTART[12] = '"';
	connectionCommands->AT_CIPSTART[13] = 'T';
	connectionCommands->AT_CIPSTART[14] = 'C';
	connectionCommands->AT_CIPSTART[15] = 'P';
	connectionCommands->AT_CIPSTART[16] = '"';
	connectionCommands->AT_CIPSTART[17] = ',';
	connectionCommands->AT_CIPSTART[18] = '"';
	connectionCommands->AT_CIPSTART[19] = '1';
	connectionCommands->AT_CIPSTART[20] = '9';
	connectionCommands->AT_CIPSTART[21] = '2';
	connectionCommands->AT_CIPSTART[22] = '.';
	connectionCommands->AT_CIPSTART[23] = '1';
	connectionCommands->AT_CIPSTART[24] = '6';
	connectionCommands->AT_CIPSTART[25] = '8';
	connectionCommands->AT_CIPSTART[26] = '.';
	connectionCommands->AT_CIPSTART[27] = '0';
	connectionCommands->AT_CIPSTART[28] = '.';
	connectionCommands->AT_CIPSTART[29] = '1';
	connectionCommands->AT_CIPSTART[30] = '"';
	connectionCommands->AT_CIPSTART[31] = ',';
	connectionCommands->AT_CIPSTART[32] = '"';
	connectionCommands->AT_CIPSTART[33] = '8';
	connectionCommands->AT_CIPSTART[34] = '0';
	connectionCommands->AT_CIPSTART[35] = '0';
	connectionCommands->AT_CIPSTART[36] = '0';
	connectionCommands->AT_CIPSTART[37] = '"';
	connectionCommands->AT_CIPSTART[38] = '\r';
	connectionCommands->AT_CIPSTART[39] = '\n';

	connectionCommands->AT_CIPSEND[0] = 'A';
	connectionCommands->AT_CIPSEND[1] = 'T';
	connectionCommands->AT_CIPSEND[2] = '+';
	connectionCommands->AT_CIPSEND[3] = 'C';
	connectionCommands->AT_CIPSEND[4] = 'I';
	connectionCommands->AT_CIPSEND[5] = 'P';
	connectionCommands->AT_CIPSEND[6] = 'S';
	connectionCommands->AT_CIPSEND[7] = 'E';
	connectionCommands->AT_CIPSEND[8] = 'N';
	connectionCommands->AT_CIPSEND[9] = 'D';
	connectionCommands->AT_CIPSEND[10] = '\r';
	connectionCommands->AT_CIPSEND[11] = '\n';

	connectionCommands->AT_CIPCLOSE[0] = 'A';
	connectionCommands->AT_CIPCLOSE[1] = 'T';
	connectionCommands->AT_CIPCLOSE[2] = '+';
	connectionCommands->AT_CIPCLOSE[3] = 'C';
	connectionCommands->AT_CIPCLOSE[4] = 'I';
	connectionCommands->AT_CIPCLOSE[5] = 'P';
	connectionCommands->AT_CIPCLOSE[6] = 'C';
	connectionCommands->AT_CIPCLOSE[7] = 'L';
	connectionCommands->AT_CIPCLOSE[8] = 'O';
	connectionCommands->AT_CIPCLOSE[9] = 'S';
	connectionCommands->AT_CIPCLOSE[10] = 'E';
}
