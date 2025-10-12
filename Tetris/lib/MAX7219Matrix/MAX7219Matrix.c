#include "MAX7219Matrix.h"
#include "SimpleSPI.h"

#define MODULES 4

static uint8_t g_devices = MODULES;

// Буфер: 8 рядків × N модулів (кожен модуль = “поверх” 8 пікселів по вертикалі)
static uint8_t buffer[8][MODULES];

// ---------------- CS ----------------
static inline void cs_low(void) { MAX7219_CS_PORT &= ~_BV(MAX7219_CS_BIT); }
static inline void cs_high(void) { MAX7219_CS_PORT |= _BV(MAX7219_CS_BIT); }

// Надсилання однієї і тієї ж команди на всі модулі
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

// ---------------- INIT ----------------
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
        max7219_set_row_all(row, 0x00);
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

// ---------------- BUFFER ----------------
void matrix_clear_buffer(void)
{
    for (uint8_t row = 0; row < 8; row++)
        for (uint8_t m = 0; m < g_devices; m++)
            buffer[row][m] = 0x00;
}

// x: 0..7 (ширина), y: 0..31 (висота зверху вниз)
// кожен модуль показує свій блок 8 рядків: y/8 -> № модуля, y%8 -> локальний рядок у модулі
void matrix_set_pixel(uint8_t x, uint8_t y, uint8_t on)
{
    if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
        return;

    uint8_t module = y / 8;   // 0..3
    module = (g_devices - 1) - module;

    uint8_t localRow = y % 8; // 0..7

    // Якщо фізично верхній модуль у каскаді ближче до MCU, можливо треба інвертувати:
    // module = (g_devices - 1) - module;  // розкоментуйте, якщо порядок модулів зверху-вниз “перевернутий”

    if (on)
        buffer[localRow][module] |= (1 << (7 - x)); // x -> біт у байті (MSB ліворуч)
    else
        buffer[localRow][module] &= ~(1 << (7 - x));
}

void matrix_draw(void)
{
    for (uint8_t col = 0; col < 8; col++)
    {
        cs_low();

        // Порядок каскаду: від найдальшого модуля
        for (int8_t m = g_devices - 1; m >= 0; --m)
        {
            uint8_t colData = 0;

            // Транспонування + вертикальне перевертання + горизонтальне дзеркалення
            for (uint8_t row = 0; row < 8; row++)
            {
                // Беремо біт із buffer[7 - row][m], але вже без (7 - col)
                // щоб інвертувати горизонталь
                if (buffer[7 - row][m] & (1 << col))
                    colData |= (1 << row);
            }

            spi_transfer(col + 1);   // адреса 1..8
            spi_transfer(colData);
        }

        cs_high();
    }
}

