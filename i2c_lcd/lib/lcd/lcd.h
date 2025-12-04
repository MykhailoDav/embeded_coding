#ifndef __LCD_H__
#define __LCD_H__

#define LCD_ADDR 0x4E

uint8_t LCD_Init(uint8_t addr);
void LCD_Clear(void);
void LCD_SetXY(uint8_t x, uint8_t y);
void LCD_Print(uint8_t x, uint8_t y, char *str);
void Lcd_SendChar(char data);
void Lcd_BackLightOnOff(uint8_t val);

#endif