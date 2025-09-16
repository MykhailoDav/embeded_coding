#include <avr/io.h>
#define LED PB5
int main() {
DDRB |= _BV(LED);

PORTB |= 1 << LED;
return 0;
}