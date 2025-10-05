#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SEG_A (1 << PD0)
#define SEG_B (1 << PD1)
#define SEG_C (1 << PD2)
#define SEG_D (1 << PD3)
#define SEG_E (1 << PD4)
#define SEG_F (1 << PD5)
#define SEG_G (1 << PD6)

#define SEG_MASK (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)

#define DIGIT_0 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define DIGIT_1 (SEG_B | SEG_C)
#define DIGIT_2 (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define DIGIT_3 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G)
#define DIGIT_4 (SEG_B | SEG_C | SEG_F | SEG_G)
#define DIGIT_5 (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define DIGIT_6 (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define DIGIT_7 (SEG_A | SEG_B | SEG_C)
#define DIGIT_8 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define DIGIT_9 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)

static const uint8_t DIGITS[10] = {
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9};

void init_segments(void)
{
    DDRD |= SEG_MASK;
    PORTD &= ~SEG_MASK;
}

void clear_segments(void)
{
    PORTD &= ~SEG_MASK;
}

void light_segments(uint8_t segments)
{
    PORTD = (PORTD & ~SEG_MASK) | (segments & SEG_MASK);
}

void setup()
{
    init_segments();
    UCSR0B = 0;
}

void loop()
{
    for (uint8_t i = 0; i < 10; ++i)
    {
        light_segments(DIGITS[i]);
        _delay_ms(300);
        clear_segments();
        _delay_ms(70);
    }

    light_segments(SEG_A);
    _delay_ms(200);
    light_segments(SEG_B);
    _delay_ms(200);
    light_segments(SEG_C);
    _delay_ms(200);
    light_segments(SEG_D);
    _delay_ms(200);
    light_segments(SEG_E);
    _delay_ms(200);
    light_segments(SEG_F);
    _delay_ms(200);

    clear_segments();
    _delay_ms(300);
}

int main(void)
{
    setup();

    for (;;)
    {
        loop();
    }
}
