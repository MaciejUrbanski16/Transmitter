#include "stm32f4xx_hal.h"

#define SLAVE_ADDRESS_LCD 0x4E

extern I2C_HandleTypeDef hi2c1;

void lcdInit (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

void updateLCD(char *str);
