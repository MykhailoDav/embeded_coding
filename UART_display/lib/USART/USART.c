#include "USART.h"
#include <string.h>
#include <stdarg.h>

#define RX_BUF_SIZE 64

static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t rx_buf_len = 0;
static USART_callback_t m_usart_callback = NULL;

void USART_Init(uint32_t baud)
{
    // Розрахунок швидкості (U2X = 0)
    UBRR0H = (uint8_t)((F_CPU / (16UL * baud) - 1) >> 8);
    UBRR0L = (uint8_t)(F_CPU / (16UL * baud) - 1);
    UCSR0A = 0;

    // Включити приймач і передавач
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // 8 біт, 1 стоп-біт, без парності
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_PutChar(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

uint8_t USART_Receive(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
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
    uint8_t ch = USART_Receive();
    USART_PutChar(ch); // echo
    return ch;
}

FILE uart_stream = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

void USART_SetStdStreams(void)
{
    stdin = stdout = &uart_stream;
}

void USART_SetCallback(USART_callback_t hnd)
{
    m_usart_callback = hnd;
}

void USART_poll(void)
{
    static char inputBuffer[128];
    static char lastCommand[128];
    static uint8_t inputLength = 0;

    //Enter
    if (UCSR0A & (1 << RXC0))
    {
        uint8_t ch = UDR0;

        if (ch == '\r' || ch == '\n')
        {
            if (inputLength > 0)
            {
                inputBuffer[inputLength] = '\0';
                printf("\r\n");

                if (m_usart_callback)
                    m_usart_callback((uint8_t *)inputBuffer);

                strncpy(lastCommand, inputBuffer, sizeof(lastCommand) - 1);
                inputLength = 0;
            }
            else
            {
                printf("\r\n$: ");
            }
        }
        

        // BACKSPACE
        else if (ch == 0x08 || ch == 0x7F)
        {
            if (inputLength > 0)
            {
                inputLength--;
                inputBuffer[inputLength] = '\0';
                printf("\b \b");
            }
        }

        // ARROW UP
        else if (ch == 0x1B)
        {
            while (!(UCSR0A & (1 << RXC0)))
                ;
            if (UDR0 == '[')
            {
                while (!(UCSR0A & (1 << RXC0)))
                    ;
                if (UDR0 == 'A')
                {
                    while (inputLength--)
                        printf("\b \b");
                    inputLength = 0;

                    strncpy(inputBuffer, lastCommand, sizeof(inputBuffer) - 1);
                    inputLength = strlen(inputBuffer);
                    printf("%s", inputBuffer);
                }
            }
        }
        else
        {
            if (inputLength < sizeof(inputBuffer) - 1)
            {
                USART_PutChar(ch); // echo
                inputBuffer[inputLength++] = ch;
            }
        }
    }
}
