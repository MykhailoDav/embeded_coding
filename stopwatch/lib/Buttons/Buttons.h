#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <stddef.h>

typedef struct
{
    bool pressed;
    bool released;
    bool long_press;
    bool hold;
    uint32_t press_start;
    uint32_t last_hold_time;
    bool was_pressed;
} ButtonState;

typedef struct
{
    ButtonState *state;
    uint8_t pin;
} ButtonDef;

void Buttons_Init(ButtonDef *buttons, uint8_t count);
void Buttons_Update(uint32_t millis);
