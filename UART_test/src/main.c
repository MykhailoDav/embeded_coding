#include <avr/io.h>
#include <stdint.h>
#include "UART.h"

void print_string(const char *str)
{
  while (*str)
  {
    USART_PutChar(*str++);
  }
}
//======================================
int main(void)
{
  USART_Init(9600);
  USART_SetStdStreams();

  printf("Hello World\n");

  int a;
  scanf("%d", &a);
  printf("You entered: %d\n", a);
  // main loop =============
  for (;;)
  {
    USART_PutChar(USART_Receive());
  }
  return 0;
}