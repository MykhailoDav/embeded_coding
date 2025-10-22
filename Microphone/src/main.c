#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"

void ADC_Init(void)
{
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}

void setup(void)
{
    USART_Init(9600);
    USART_SetStdStreams();
    ADC_Init();

    DDRD &= ~(1 << PD7);
    PORTD |= (1 << PD7);

    printf("Sound sensor (D0→D7, A0) test\r\n");
    printf("=================================\r\n");
}

void loop(void)
{
    static uint32_t sound_count = 0;
    static uint8_t prev_state = 1;

    uint8_t current_state = (PIND & (1 << PD7)) ? 1 : 0;

    uint16_t analog_value = ADC_Read(0);

    if (prev_state == 1 && current_state == 0)
    {
        sound_count++;
        printf("Sound #%lu detected!\r\n", sound_count);
    }

    printf("A0 = %4u\r\n", analog_value);

    prev_state = current_state;
    _delay_ms(100);
}

int main(void)
{
    setup();
    for (;;)
        loop();
}
