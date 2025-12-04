#include <util/delay.h>
#include "lcd.h"
#include "./i2c.h"

#define LCD_PIN_RS 1
#define LCD_PIN_RW 2
#define LCD_PIN_E  4

#define LCD_CMD  0
#define LCD_DATA 1

#define LCD_DISP_CLEAR   0x01
#define LCD_DISP_OFF     0x08
#define LCD_DISP_ON      0x0C
#define LCD_CURSOR_ON    0x0E
#define LCD_CURSOR_BLINK 0x0F
#define LCD_RETURN_HOME  0x02 
#define LCD_ENTRY_MODE   0x06
#define LCD_4BIT_MODE    0x20
#define LCD_8BIT_MODE    0x30
#define LCD_2_ROWS       0x08
#define LCD_FONT_5x8     0x00
#define LCD_FONT_5x10    0x04
#define LCD_POSITION     0x80
#define LCD_SET_CGRAM    0x40


static uint8_t back_light = 1;
static uint8_t lcd_addr;

//-----------------------------------------------------------------------------
static void LCD_E_pulse (uint8_t data)
{
  I2C_Write(data | LCD_PIN_E); 
  _delay_us(1);
  I2C_Write(data);
  _delay_us(55);
}

//-----------------------------------------------------------------------------
static void LCD_Send(uint8_t addr, uint8_t d, uint8_t type)
{
  uint8_t hi_nibble = (d & 0xF0);
  uint8_t low_nibble = (d << 4);

  if (back_light) {
    hi_nibble  |= 8; 
    low_nibble |= 8;
  }
  if (type) {
    hi_nibble  |= LCD_PIN_RS; 
    low_nibble |= LCD_PIN_RS;
  }
  I2C_Start();
  I2C_Write(addr);
  // Send upper nibble, E pulse
  I2C_Write(hi_nibble);
  LCD_E_pulse(hi_nibble);
  // Send lower nibble, E pulse
  I2C_Write(low_nibble);
  LCD_E_pulse(low_nibble);
  I2C_Stop();
}

//-----------------------------------------------------------------------------
uint8_t LCD_Init(uint8_t addr)
{
  uint8_t res, data = 0;
  lcd_addr = addr;

  _delay_ms(16);
  I2C_Start();
  res = I2C_Write(addr);
  if (res != I2C_ACK) {
    I2C_Stop();
    return 1;
  }
  // DB7 BD6 DB5 DB4 P3 E RW RS 
  // DB4=1, DB5=1 / BF cannot be checked in these instructions
  data = 0x30;
  I2C_Write(data);
  LCD_E_pulse(data);
  _delay_ms(5);  // delay > 4.1ms
  // DB4=1, DB5=1 / BF cannot be checked in these instructions
  I2C_Write(data);
  LCD_E_pulse(data);
  _delay_us(110);  // delay > 100us
  // DB4=1, DB5=1 / BF cannot be checked in these instructions
  I2C_Write(data);
  LCD_E_pulse(data);  
  _delay_us(55);  // delay > 45us (=37+4 * 270/250)
  // DB5=1 / 4 bit mode 0x20 / BF cannot be checked in these instructions
  data = 0x20;
  I2C_Write(data);
  LCD_E_pulse(data);
  _delay_us(55);  // delay > 45us (=37+4 * 270/250)
  I2C_Stop();

  // 4 bit mode, 2 rows, font 5x8
  LCD_Send(addr, LCD_4BIT_MODE | LCD_2_ROWS | LCD_FONT_5x8, LCD_CMD);
  // display off 0x08 - send 8 bits in 4 bit mode
  LCD_Send(addr, LCD_DISP_OFF, LCD_CMD);
  // display clear 0x01 - send 8 bits in 4 bit mode
  LCD_Send(addr, LCD_DISP_CLEAR, LCD_CMD);
  _delay_ms(5);
  // entry mode set 0x06 - send 8 bits in 4 bit mode
  LCD_Send(addr, LCD_ENTRY_MODE, LCD_CMD);
  LCD_Send(addr, LCD_DISP_ON, LCD_CMD);
  // return success
  return 0;
}

//-----------------------------------------------------------------------------
void LCD_SetXY(uint8_t x, uint8_t y)
{
  if (x > 15) x = 15;
  if (y != 0) {
    x += 0x40;
  }   
  LCD_Send(lcd_addr, LCD_POSITION | x, LCD_CMD);
  _delay_ms(5);
}

//-----------------------------------------------------------------------------
void Lcd_SendChar(char data)
{
  // data/command -> pin RS High
  LCD_Send(lcd_addr, (uint8_t) data, LCD_DATA);
  //LCD_CheckBF(addr);
  _delay_ms(2);
}

//-----------------------------------------------------------------------------
void LCD_Clear(void) 
{
  LCD_Send(lcd_addr, LCD_DISP_CLEAR, LCD_CMD);
  _delay_ms(10);
}

//-----------------------------------------------------------------------------
void LCD_Print(uint8_t x, uint8_t y, char *str)
{
  LCD_SetXY(x, y);
  while (*str) {
    Lcd_SendChar(*str++);
  }
}

void Lcd_BackLightOnOff(uint8_t val)
{
  
  back_light = val;
  I2C_Start();
  if (I2C_Write(lcd_addr) != I2C_ACK) {
    I2C_Stop();
    return;
  }
  I2C_Write((val != 0)? 8: 0);
  I2C_Stop();
}

//-----------------------------------------------------------------------------
// Створення власного символу (0-7)
// charmap - масив з 8 байтів, де кожен байт = рядок символу 5x8
void LCD_CreateChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // маємо тільки 8 локацій 0-7
  LCD_Send(lcd_addr, LCD_SET_CGRAM | (location << 3), LCD_CMD);
  
  for (uint8_t i = 0; i < 8; i++) {
    Lcd_SendChar(charmap[i]);
  }
  
  // Повертаємося до DDRAM
  LCD_Send(lcd_addr, LCD_POSITION, LCD_CMD);
}