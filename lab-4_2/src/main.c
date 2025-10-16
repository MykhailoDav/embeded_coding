#include <avr/io.h>
#include <util/delay.h>

#define MAX7219_DECODE_MODE_REG 0x09
#define MAX7219_INTENSITY_REG 0x0a
#define MAX7219_SCAN_LIMIT_REG 0x0b
#define MAX7219_SHUTDOWN_REG 0x0c
#define MAX7219_DISPLAY_TEST_REG 0x0f
#define DATA_PIN PB3
#define SCK_PIN PB5
#define LATCH_PIN PB2

void setup_spi(void);
void spi_transmit(uint8_t data);
void send_to_max7219(uint8_t addr, uint8_t data);
void setup_max7219_led_matrix(void);

uint8_t Cat[8] = {
    0b10001000,
    0b11111000,
    0b10101000,
    0b01110001,
    0b00100001,
    0b01111001,
    0b01111101,
    0b10111110};
int main(void)
{
  setup_spi();
  setup_max7219_led_matrix();
  for (int i = 0; i < 8; i++)
  {
    send_to_max7219(i + 1, Cat[i]);
  }
}
void setup_spi(void)
{
  DDRB |= _BV(DATA_PIN) | _BV(SCK_PIN) | _BV(LATCH_PIN);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
void spi_transmit(uint8_t data)
{
  SPDR = data;
  while (bit_is_clear(SPSR, SPIF))
    ;
}
void send_to_max7219(uint8_t addr, uint8_t data)
{
  spi_transmit(addr);
  spi_transmit(data);
  PORTB |= _BV(LATCH_PIN);
  _delay_us(1);
  PORTB &= ~_BV(LATCH_PIN);
  _delay_us(1);
}
void setup_max7219_led_matrix(void)
{
  send_to_max7219(MAX7219_DECODE_MODE_REG, 0);
  send_to_max7219(MAX7219_SCAN_LIMIT_REG, 0xf);
  send_to_max7219(MAX7219_INTENSITY_REG, 0x07);
  send_to_max7219(MAX7219_DISPLAY_TEST_REG, 0);
  send_to_max7219(MAX7219_SHUTDOWN_REG, 1);
}