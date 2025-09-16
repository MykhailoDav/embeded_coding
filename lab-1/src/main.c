#include <avr/io.h>
#define LED PB5
int main() {
DDRB |= _BV(LED);

PORTB |= 1 << LED;
return 0;
}


//Blinker:
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
}