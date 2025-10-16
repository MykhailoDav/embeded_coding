#include <avr/io.h>
#include <util/delay.h>
typedef struct
{
  volatile uint8_t *port;
  uint8_t pin;
} PinDef;
PinDef row_pins[8] = {
    {&PORTD, PD4},
    {&PORTD, PD5},
    {&PORTD, PD6},
    {&PORTD, PD7},
    {&PORTC, PC0},
    {&PORTC, PC1},
    {&PORTC, PC2},
    {&PORTC, PC3},
};
PinDef col_pins[8] = {
    {&PORTB, PB0},
    {&PORTB, PB1},
    {&PORTB, PB2},
    {&PORTB, PB3},
    {&PORTB, PB4},
    {&PORTB, PB5},
    {&PORTD, PD2},
    {&PORTD, PD3}};
const uint8_t smiley_face[8] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100};
void init_matrix(void)
{
  DDRB |= 0b00111111;
  DDRD |= 0b11111100;
  DDRC |= 0b00001111;
}
void display_symbol(const uint8_t *pattern)
{
  for (uint8_t row = 0; row < 8; row++)
  {
    *(row_pins[row].port) |= _BV(row_pins[row].pin);
    for (uint8_t col = 0; col < 8; col++)
    {
      if (pattern[row] & (1 << col))
      {
        *(col_pins[col].port) &= ~_BV(col_pins[col].pin);
      }
      else
      {
        *(col_pins[col].port) |= _BV(col_pins[col].pin);
      }
    }
    _delay_ms(2);
    *(row_pins[row].port) &= ~_BV(row_pins[row].pin);
  }
}
int main(void)
{
  init_matrix();
  while (1)
  {
    display_symbol(smiley_face);
  }
}