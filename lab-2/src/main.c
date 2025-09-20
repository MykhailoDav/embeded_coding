#include <avr/io.h>
#include <util/delay.h>

#define SEG_A (1 << PD7)
#define SEG_B (1 << PB0)
#define SEG_C (1 << PB1)
#define SEG_D (1 << PB2)
#define SEG_E (1 << PB3)
#define SEG_F (1 << PB4)

void init_segments(void)
{
    DDRD |= (1 << PD7);
    DDRB |= 0x1F;
}

void clear_segments(void)
{
    PORTD &= ~(1 << PD7);
    PORTB &= ~0x1F;
}

void light_segment(uint8_t segmentD, uint8_t segmentB)
{
    clear_segments();
    PORTD |= segmentD;
    PORTB |= segmentB;
    _delay_ms(200);
}

int main(void)
{
    init_segments();

    while (1)
    {
        light_segment(SEG_A, 0);
        light_segment(0, SEG_B);
        light_segment(0, SEG_C);
        light_segment(0, SEG_D);
        light_segment(0, SEG_E);
        light_segment(0, SEG_F);
    }
}
