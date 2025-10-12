#pragma once
#include <avr/io.h>
#include <stdint.h>

// Ініціалізація апаратного SPI: Master, F_CPU/16
static inline void spi_begin(void) {
    // MOSI (PB3, D11) та SCK (PB5, D13) — виходи
    DDRB |= _BV(DDB3) | _BV(DDB5);
    // Увімкнути SPI, Master, дільник /16: SPR0=1, SPR1=0, SPI2X=0
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
    SPSR &= (uint8_t)~_BV(SPI2X);
}

// Передати 1 байт по SPI та дочекатися завершення
static inline uint8_t spi_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & _BV(SPIF))) { ; }
    return SPDR;
}