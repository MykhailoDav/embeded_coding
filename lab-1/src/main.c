#include <avr/io.h>
#include <util/delay.h>

#define R_PIN PC3
#define G_PIN PC2
#define B_PIN PC1

int main(void)
{
    DDRC |= (1 << R_PIN) | (1 << G_PIN) | (1 << B_PIN);

    while (1)
    {
        PORTC |= (1 << R_PIN);
        PORTC &= ~((1 << G_PIN) | (1 << B_PIN));
        _delay_ms(500);

        PORTC |= (1 << G_PIN);
        PORTC &= ~((1 << R_PIN) | (1 << B_PIN));
        _delay_ms(500);

        PORTC |= (1 << B_PIN);
        PORTC &= ~((1 << R_PIN) | (1 << G_PIN));
        _delay_ms(500);
    }

    return 0;
}
