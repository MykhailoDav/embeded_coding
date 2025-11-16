#include <avr/io.h>
#include <util/delay.h>
#define PCF8574_ADDRESS 0x27
#define BACKLIGHT 0x08
#define ENABLE 0x04
#define RS 0x01
void i2cInit(void);
void i2cStart(void);
void i2cWrite(uint8_t data);
void i2cStop(void);
void lcdWriteNibble(uint8_t nibble, uint8_t mode);
void lcdSend(uint8_t data, uint8_t mode);
void lcdCommand(uint8_t cmd);
void lcdData(uint8_t data);
void lcdInit(void);
void lcdPrint(const char *str);
void lcdSetCursor(uint8_t col, uint8_t row);
int main(void)
{
  i2cInit();
  lcdInit();
  lcdSetCursor(0, 0);
  lcdPrint("Hello, World!");
  lcdSetCursor(0, 1);
  lcdPrint("I2C LCD1602");
  while (1)
    ;
}
void i2cInit(void)
{
  TWSR = 0x00;
  TWBR = 0x48;
  TWCR = (1 << TWEN);
}
void i2cStart(void)
{
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)))
    ;
}
void i2cWrite(uint8_t data)
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)))
    ;
}
void i2cStop(void)
{
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  _delay_us(10);
}
void lcdWriteNibble(uint8_t nibble, uint8_t mode)
{
  uint8_t data = nibble | BACKLIGHT | mode;
  i2cStart();
  i2cWrite(PCF8574_ADDRESS << 1);
  i2cWrite(data | ENABLE);
  _delay_us(1);
  i2cWrite(data & ~ENABLE);
  i2cStop();
}
void lcdSend(uint8_t data, uint8_t mode)
{
  lcdWriteNibble(data & 0xF0, mode);
  lcdWriteNibble((data << 4) & 0xF0, mode);
  _delay_us(50);
}
void lcdCommand(uint8_t cmd)
{
  lcdSend(cmd, 0);
}
void lcdData(uint8_t data)
{
  lcdSend(data, RS);
}
void lcdInit(void)
{
  _delay_ms(50);
  lcdWriteNibble(0x30, 0);
  _delay_ms(5);
  lcdWriteNibble(0x30, 0);
  _delay_us(150);
  lcdWriteNibble(0x30, 0);
  _delay_us(150);
  lcdWriteNibble(0x20, 0);
  _delay_us(150);
  lcdCommand(0x28);
  lcdCommand(0x08);
  lcdCommand(0x01);
  _delay_ms(2);
  lcdCommand(0x06);
  lcdCommand(0x0C);
}
void lcdSetCursor(uint8_t col, uint8_t row)
{
  uint8_t row_offsets[] = {0x00, 0x40};
  lcdCommand(0x80 | (col + row_offsets[row]));
}
void lcdPrint(const char *str)
{
  while (*str)
  {
    lcdData(*str++);
  }
}