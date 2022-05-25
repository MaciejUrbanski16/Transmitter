
#ifndef INC_STARTUP_H_
#define INC_STARTUP_H_

volatile uint8_t measurementStarted = 0;

void initialiseBeforeStart()
{

	char startUp1[] = ">";
	char startUp2[] = ">>";
	char startUp3[] = ">>>";
	char startUp4[] = ">>>>";
	char startUp5[] = " >>>>";
	char startUp6[] = "  >>>>";
	char startUp7[] = "   >>>>";
	char startUp8[] = "    >>>>";
	char startUp9[] = "     >>>>";
	char startUp10[] = "      >>>>";
	char startUp11[] = "       >>>>";
	char startUp12[] = "        >>>>";
	char startUp13[] = "         >>>>";
	char startUp14[] = "          >>>>";
	char startUp15[] = "           >>>>";
	char startUp16[] = "            >>>>";
	char startUp17[] = "             >>>";
	char startUp18[] = "              >>";
	char startUp19[] = "               >";

//	char initDisplayContent[3][3]; //startUp1, startUp1, startUp1};
	//initDisplayContent[0] = startUp1;

    lcdClear();
    lcdSetCursor(0, 0);
    lcdSendString (startUp1);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp2);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp3);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp4);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp5);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp6);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp7);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp8);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp9);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp10);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp11);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp12);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp13);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp14);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp15);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp16);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp17);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp18);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;

    lcdSetCursor(0, 0);
    lcdSendString (startUp19);
    HAL_Delay(10);
    lcdClear();
    if(measurementStarted == 1) return;
}


#endif /* INC_STARTUP_H_ */
