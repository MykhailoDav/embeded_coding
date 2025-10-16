#include "SimpleSPI.h"

void spi_begin(void)
{
    // MOSI (PB3, D11) and SCK (PB5, D13) — outputs
    DDRB |= _BV(DIN_PIN) | _BV(CLK_PIN);
    // Enable SPI, Master, speed fck/16: SPR0=1, SPR1=0, SPI2X=0
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & _BV(SPIF)))
    {
        ;
    }
    return SPDR;
}