#include "UART.h"

void USART_Init(uint32_t baud)
{
    // Розрахунок швидкості(U2X = 0)
    UBRR0H = (uint8_t)((F_CPU / (16 * baud) - 1) >> 8);
    UBRR0L = (uint8_t)(F_CPU / (16 * baud) - 1);
    UCSR0A = 0;
    // Включити передавач та приймач
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Налаштування фрейму передачі/прийому даних
    // 8-біт даних, 1-стоп біт, без перевірки парності
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_PutChar(uint8_t data)
{
    // Чекаємо завершення передачі даних
    while (!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    // Почати передачу даних
    UDR0 = data;
}

uint8_t USART_Receive(void)
{
    // Чекаємо завершення прийому даних
    while (!(UCSR0A & (1 << RXC0)))
    {
        ;
    }
    // Повертаємо прийняті дані
    return UDR0;
}

int uart_putch(char ch, FILE *stream)
{
    if (ch == '\n')
        USART_PutChar('\r');
    USART_PutChar(ch);
    return 0;
}

int uart_getch(FILE *stream)
{
    uint8_t ch;
    ch = USART_Receive();
    // Echo the Output Back to terminal
    USART_PutChar(ch);
    return ch;
}

FILE uart_stream = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

void USART_SetStdStreams(void)
{
    stdin = stdout = &uart_stream;
}
