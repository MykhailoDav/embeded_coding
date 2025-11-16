#include "lcd1602_i2c.h"
#include "i2c_bitbang.h"
#include <util/delay.h>

#define LCD_RS        (1 << 5)   // P5
#define LCD_ENABLE    (1 << 4)   // P4
#define LCD_BACKLIGHT (1 << 7)   // P7

static void LCD_WriteNibble(uint8_t nibble, uint8_t rs)
{
    uint8_t data = nibble | LCD_BACKLIGHT;
    if (rs)
        data |= LCD_RS;

    I2C_Start();
    I2C_Write(LCD_I2C_ADDR << 1);
    I2C_Write(data | LCD_ENABLE);
    _delay_us(1);
    I2C_Write(data & ~LCD_ENABLE);
    I2C_Stop();
}

static void LCD_Send(uint8_t data, uint8_t rs)
{
    LCD_WriteNibble(data & 0xF0, rs);
    LCD_WriteNibble((data << 4) & 0xF0, rs);
}

void LCD_SendCmd(uint8_t cmd)
{
    LCD_Send(cmd, 0);
}

void LCD_SendChar(char c)
{
    LCD_Send(c, 1);
}

void LCD_SendString(const char *str)
{
    while (*str)
    {
        LCD_SendChar(*str++);
    }
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    LCD_SendCmd(0x80 | (addr + col));
}

void LCD_Init(void)
{
    _delay_ms(50);

    LCD_WriteNibble(0x30, 0);
    _delay_ms(5);
    LCD_WriteNibble(0x30, 0);
    _delay_us(100);
    LCD_WriteNibble(0x30, 0);
    _delay_us(100);
    LCD_WriteNibble(0x20, 0);
    _delay_us(100);

    LCD_SendCmd(0x28);
    LCD_SendCmd(0x08);
    LCD_SendCmd(0x01);
    _delay_ms(2);
    LCD_SendCmd(0x06);
    LCD_SendCmd(0x0C);
}
