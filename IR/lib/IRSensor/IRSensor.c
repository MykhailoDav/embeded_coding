#include "IRSensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define IR_PIN PD2        // Вхід ІЧ-приймача
#define LED_PIN PB5       // Вбудований світлодіод (Arduino pin 13)
#define IR_INT_VECTOR INT0_vect

void IR_Init(void)
{
    // === Налаштування пінів ===
    DDRD &= ~(1 << IR_PIN);   // PD2 як вхід
    PORTD |= (1 << IR_PIN);   // підтяжка до 1 (якщо треба)

    DDRB |= (1 << LED_PIN);   // PB5 як вихід для LED
    PORTB &= ~(1 << LED_PIN); // вимкнути LED

    // === Налаштування переривання INT0 ===
    EICRA = (1 << ISC00);     // спрацьовує на будь-яку зміну рівня
    EIMSK = (1 << INT0);      // дозволити INT0

    sei();                    // глобально дозволити переривання
}

// === Обробник переривання ===
ISR(IR_INT_VECTOR)
{
    // Просто інвертуємо LED при кожній зміні рівня сигналу з приймача
    PORTB ^= (1 << LED_PIN);
}
