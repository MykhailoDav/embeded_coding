#include "MAX7219Matrix.h"
#include "../SimpleSPI/SimpleSPI.h"

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define MODULES 4

static uint8_t g_devices = MODULES;
static uint8_t buffer[MATRIX_HEIGHT][MATRIX_WIDTH / 8];

static inline void cs_low(void)
{
    MAX7219_CS_PORT &= ~_BV(MAX7219_CS_BIT);
}

static inline void cs_high(void)
{
    MAX7219_CS_PORT |= _BV(MAX7219_CS_BIT);
}

static void send_all(uint8_t addr, uint8_t data)
{
    cs_low();
    for (uint8_t i = 0; i < g_devices; i++)
    {
        spi_transfer(addr);
        spi_transfer(data);
    }
    cs_high();
}

void max7219_init(uint8_t devices, uint8_t intensity)
{
    g_devices = devices;
    MAX7219_CS_DDR |= _BV(MAX7219_CS_BIT);
    MAX7219_CS_PORT |= _BV(MAX7219_CS_BIT);

    spi_begin();

    send_all(MAX7219_DISPLAYTEST, 0x00);
    send_all(MAX7219_DECODE_MODE, 0x00);
    send_all(MAX7219_SCAN_LIMIT, 0x07);
    send_all(MAX7219_INTENSITY, intensity & 0x0F);
    send_all(MAX7219_SHUTDOWN, 0x01);

    max7219_clear();
    matrix_clear_buffer();
}

void max7219_clear(void)
{
    for (uint8_t row = 1; row <= 8; row++)
    {
        max7219_set_row_all(row, 0x00);
    }
}

void max7219_set_row_all(uint8_t row1to8, uint8_t pattern)
{
    if (row1to8 < 1 || row1to8 > 8)
        return;

    cs_low();
    for (uint8_t i = 0; i < g_devices; i++)
    {
        spi_transfer(row1to8);
        spi_transfer(pattern);
    }
    cs_high();
}

void max7219_send_to_device(uint8_t idx, uint8_t addr, uint8_t data)
{
    if (idx >= g_devices)
        return;

    cs_low();
    for (int8_t i = g_devices - 1; i >= 0; --i)
    {
        if (i == idx)
        {
            spi_transfer(addr);
            spi_transfer(data);
        }
        else
        {
            spi_transfer(MAX7219_NOP);
            spi_transfer(0x00);
        }
    }
    cs_high();
}

void matrix_clear_buffer(void)
{
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
    {
        for (uint8_t bx = 0; bx < MATRIX_WIDTH / 8; bx++)
        {
            buffer[y][bx] = 0x00;
        }
    }
}

void matrix_set_pixel(uint8_t x, uint8_t y, uint8_t on)
{
    if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
        return;

    uint8_t block = x / 8;
    uint8_t bit = x % 8;

    if (on)
        buffer[y][block] |= (1 << (7 - bit));
    else
        buffer[y][block] &= ~(1 << (7 - bit));
}

void matrix_draw(void)
{
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
    {
        cs_low();
        for (int8_t dev = g_devices - 1; dev >= 0; --dev)
        {
            uint8_t data = buffer[7 - y][dev];

            uint8_t mirrored = 0;
            for (uint8_t b = 0; b < 8; b++)
            {
                if (data & (1 << b))
                    mirrored |= (1 << (7 - b));
            }

            spi_transfer(y + 1);
            spi_transfer(mirrored);
        }
        cs_high();
    }
}
