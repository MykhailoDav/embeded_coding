#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "USART.h"
#define LED_PIN PB5
// ===== USART callback function =====
void uart_line_relive(uint8_t *buf)
{
    if (strcmp((char *)buf, "led on") == 0)
    {
        PORTB |= (1 << LED_PIN);
        printf("\nLED ON\r\n");
    }
    else if (strcmp((char *)buf, "led off") == 0)
    {
        PORTB &= ~(1 << LED_PIN);
        printf("\nLED OFF\r\n");
    }
    else if (strcmp((char *)buf, "toggle") == 0)
    {
        PORTB ^= (1 << LED_PIN);
        printf("\nLED TOGGLED\r\n");
    }
    else
    {
        printf("\nUnknown command: %s\r\n", buf);
    }
}

// ===== Main =====
int main(void)
{
    DDRB |= (1 << LED_PIN);

    USART_Init(9600);
    USART_SetStdStreams();
    USART_SetCallback(uart_line_relive);

    printf("System ready. Type 'led on', 'led off', or 'toggle'\r\n");

    while (1)
    {
        USART_poll(); 
        _delay_ms(1);
    }
}
