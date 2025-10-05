#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include "Display.h"
#include "Buttons.h"

#define BTN_START PD2
#define BTN_STOP PD3
#define BTN_RESET PD4

volatile uint32_t millis = 0;

typedef enum
{
  STATE_NORMAL,
  STATE_MENU
} State;

State state = STATE_NORMAL;

uint16_t speed_percent = 150;
uint16_t tick_time = 1000;

uint32_t last_inc = 0, last_dec = 0, last_update = 0;

ButtonState btn_start = {0};
ButtonState btn_stop = {0};
ButtonState btn_reset = {0};

int main(void)
{
  display_init();

  // Настройка кнопок
  DDRD &= ~((1 << BTN_START) | (1 << BTN_STOP) | (1 << BTN_RESET));
  PORTD |= (1 << BTN_START) | (1 << BTN_STOP) | (1 << BTN_RESET);

  ButtonDef buttons[] = {
      {&btn_start, BTN_START},
      {&btn_stop, BTN_STOP},
      {&btn_reset, BTN_RESET}};
  Buttons_Init(buttons, 3);

  uint16_t counter = 0;
  bool running = false;
  bool menu_intro_done = false;
  uint32_t menu_intro_start = 0;

  while (1)
  {
    _delay_ms(1);
    millis++;

    Buttons_Update(millis);

    tick_time = 2000 - (speed_percent * 10);
    if (tick_time < 10)
      tick_time = 10;

    // -----------------------
    // RESET
    // -----------------------
    if (btn_reset.long_press)
    {
      if (state == STATE_NORMAL)
        counter = 0;
      else
        speed_percent = 100;
    }
    else if (btn_reset.released && (millis - btn_reset.press_start <= 2000))
    {
      if (state == STATE_NORMAL)
      {
        state = STATE_MENU;
        menu_intro_done = false;
        menu_intro_start = millis;
      }
      else
      {
        state = STATE_NORMAL;
      }
    }

    // -----------------------
    // MAIN LOGIC
    // -----------------------
    if (state == STATE_NORMAL)
    {
      if (btn_start.pressed)
        running = true;
      if (btn_stop.pressed)
        running = false;

      if (running && (millis - last_update >= tick_time))
      {
        last_update = millis;
        if (counter < 9999)
          counter++;
        else
          running = false;
      }

      if (!running && (millis / 500) % 2 == 0)
        display_off();
      else
        display_number(counter);
    }
    else if (state == STATE_MENU)
    {
      if (!menu_intro_done)
      {
        display_word("bEEr");
        if (millis - menu_intro_start > 1000)
          menu_intro_done = true;
      }
      else
      {
        if (btn_stop.pressed || btn_stop.hold)
        {
          if (speed_percent < 200)
            speed_percent += 5;
        }

        if (btn_start.pressed || btn_start.hold)
        {
          if (speed_percent > 1)
            speed_percent -= 5;
        }

        display_number(speed_percent);
      }
    }
  }
}
