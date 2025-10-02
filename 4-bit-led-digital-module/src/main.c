#include <avr/io.h>
#include <util/delay.h>

// Визначення ліній (припустимо PD2=SCLK, PD3=RCLK, PD4=DIO)
#define SCLK PD2
#define RCLK PD3
#define DIO PD4

// Макроси для встановлення/скидання
#define SET(port, bit) ((port) |= (1 << (bit)))
#define CLR(port, bit) ((port) &= ~(1 << (bit)))

// Таблиця сегментів для 0-9 (спільний катод, під 74HC595)
uint8_t segCode[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Ініціалізація портів
void init_shift(void)
{
  DDRD |= (1 << SCLK) | (1 << RCLK) | (1 << DIO);
}

// Відправка одного біта у 74HC595
void shift_bit(uint8_t bit)
{
  if (bit)
    SET(PORTD, DIO);
  else
    CLR(PORTD, DIO);

  // Такт
  CLR(PORTD, SCLK);
  _delay_us(1);
  SET(PORTD, SCLK);
}

// Відправка байта у 74HC595
void shift_byte(uint8_t data)
{
  for (int i = 7; i >= 0; i--)
  {
    shift_bit(data & (1 << i));
  }
}

// Засувка
void latch(void)
{
  CLR(PORTD, RCLK);
  _delay_us(1);
  SET(PORTD, RCLK);
}

// Виведення однієї цифри
void display_digit(uint8_t digit)
{
  shift_byte(segCode[digit]);
  latch();
}

int main(void)
{
  init_shift();
  display_digit(0);

  /* while (1) {
      for (uint8_t i = 0; i < 10; i++) {
          display_digit(i);
          _delay_ms(500);
      }
  } */
}
