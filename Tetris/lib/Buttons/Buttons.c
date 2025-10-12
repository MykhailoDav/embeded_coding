#include "Buttons.h"

static ButtonDef *btn_list = NULL;
static uint8_t btn_count = 0;

static const uint16_t long_press_time = 800; // 0.8 с
static const uint16_t hold_start_delay = 500;
static const uint16_t hold_initial_delay = 200;
static const uint16_t hold_min_delay = 50;

void Buttons_Init(ButtonDef *buttons, uint8_t count)
{
    btn_list = buttons;
    btn_count = count;
}

void Buttons_Update(uint32_t millis)
{
    for (uint8_t i = 0; i < btn_count; i++)
    {
        ButtonState *b = btn_list[i].state;
        uint8_t pin = btn_list[i].pin;

        bool is_pressed = !(PIND & (1 << pin));

        b->pressed = false;
        b->released = false;
        b->long_press = false;
        b->hold = false;

        // перше натискання
        if (is_pressed && !b->was_pressed)
        {
            b->pressed = true;
            b->press_start = millis;
            b->last_hold_time = millis;
        }
        // відпускання
        else if (!is_pressed && b->was_pressed)
        {
            b->released = true;
        }
        // утримання
        else if (is_pressed && b->was_pressed)
        {
            // Довге утримання (>2 c)
            if (millis - b->press_start > long_press_time)
                b->long_press = true;

            if (millis - b->press_start > hold_start_delay)
            {
                uint16_t hold_time = millis - b->press_start;
                uint16_t hold_delay = hold_initial_delay;

                // Плавне прискорення
                if (hold_time > 1500)
                    hold_delay = hold_min_delay;
                else if (hold_time > 1000)
                    hold_delay = 100;

                if (millis - b->last_hold_time >= hold_delay)
                {
                    b->hold = true;
                    b->last_hold_time = millis;
                }
            }
        }

        b->was_pressed = is_pressed;
    }
}
