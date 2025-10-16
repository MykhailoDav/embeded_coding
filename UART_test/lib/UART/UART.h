#pragma once
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

void USART_Init(uint32_t baud);
void USART_PutChar(uint8_t data);
uint8_t USART_Receive(void);

void USART_SetStdStreams(void);