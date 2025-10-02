#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BTN_INC PB0
#define BTN_RST PB1
#define SEG_PORT PORTD

#define SHORT_PRESS_TIME 3000
#define AUTO_REPEAT_DELAY 1000
volatile uint32_t millis = 0;
volatile uint8_t counter = 0;

uint8_t seg_digits[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};

void display_digit(uint8_t num)
{
  if (num <= 9)
  {
    SEG_PORT = seg_digits[num];
  }
}

void scan_button_inc();
void scan_button_rst();

int main(void)
{
  DDRD = 0xFF;
  DDRB &= ~((1 << BTN_INC) | (1 << BTN_RST));
  PORTB |= (1 << BTN_INC) | (1 << BTN_RST);

  display_digit(counter);

  while (true)
  {
    _delay_ms(1);
    millis++;

    scan_button_inc();
    scan_button_rst();
  }
  return 0;
}

void scan_button_inc()
{
  static bool is_pressed = false;
  static uint32_t press_start = 0;
  static uint32_t last_repeat = 0;
  uint32_t current_time = millis;

  if (!(PINB & (1 << BTN_INC)))
  {
    if (!is_pressed)
    {
      is_pressed = true;
      press_start = current_time;
    }
    else
    {
      if (current_time - press_start > SHORT_PRESS_TIME)
      {
        if (current_time - last_repeat >= AUTO_REPEAT_DELAY)
        {
          last_repeat = current_time;
          if (counter < 9)
          {
            counter++;
            display_digit(counter);
          }
        }
      }
    }
  }
  else
  {
    if (is_pressed)
    {
      if (current_time - press_start <= SHORT_PRESS_TIME)
      {
        if (counter < 9)
        {
          counter++;
          display_digit(counter);
        }
      }
      is_pressed = false;
    }
  }
}

void scan_button_rst()
{
  static bool is_pressed = false;
  static uint32_t last_time = 0;
  uint32_t current_time = millis;

  if (!(PINB & (1 << BTN_RST)))
  {
    if (!is_pressed && current_time - last_time > 200)
    {
      counter = 0;
      display_digit(counter);
      is_pressed = true;
      last_time = current_time;
    }
  }
  else
  {
    is_pressed = false;
  }
}
