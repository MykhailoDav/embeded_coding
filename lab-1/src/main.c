#include <avr/io.h>
#include <util/delay.h>

#define LED PB5

void dot()
{
    PORTB |= (1 << LED);
    _delay_ms(200);
    PORTB &= ~(1 << LED);
    _delay_ms(200);
}

void dash()
{
    PORTB |= (1 << LED);
    _delay_ms(600);
    PORTB &= ~(1 << LED);
    _delay_ms(200);
}

int main(void)
{
    DDRB |= (1 << LED);

    while (1)
    {
        // S
        dot();
        dot();
        dot();
        _delay_ms(600);

        // O
        dash();
        dash();
        dash();
        _delay_ms(600);

        // S
        dot();
        dot();
        dot();

        _delay_ms(1400);
    }
}

/* #include <avr/io.h>
#define LED PB5
int main() {
DDRB |= _BV(LED);

PORTB |= 1 << LED;
return 0;
}
 */

/* //Blinker:
#include <avr/io.h>
#include <util/delay.h>

#define LED PB5

int main(void) {
    DDRB |= _BV(LED);

    while (1) {
        PORTB |= (1 << LED);
        _delay_ms(500);
        PORTB &= ~(1 << LED);
        _delay_ms(500);
    }

    return 0;
} */