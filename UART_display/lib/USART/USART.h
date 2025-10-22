#pragma once
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

// ====================== CALLBACK ======================
typedef void (*USART_callback_t)(uint8_t *);

// ====================== API ======================
void USART_Init(uint32_t baud);
void USART_PutChar(uint8_t data);
uint8_t USART_Receive(void);

void USART_SetStdStreams(void);
void USART_poll(void);
void USART_SetCallback(USART_callback_t hnd);