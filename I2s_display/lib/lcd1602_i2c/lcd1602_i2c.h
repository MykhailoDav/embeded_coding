#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include <avr/io.h>

#define LCD_I2C_ADDR 0x27 // PCF8574 A0,A1,A2 = 1

// SDA    --->  PC4 (A4)
// SCL    --->  PC5 (A5)
void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendChar(char c);
void LCD_SendString(const char *str);
void LCD_SetCursor(uint8_t col, uint8_t row);

#endif
