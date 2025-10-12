#pragma once
#include <avr/io.h>
#include <stdint.h>

#define DIN_PIN PB3 // D11
#define CLK_PIN PB5 // D13

// Hardware SPI initialization: Master, F_CPU/16
void spi_begin(void);

// Передати 1 байт по SPI та дочекатися завершення
uint8_t spi_transfer(uint8_t data);