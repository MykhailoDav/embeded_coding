#include "Display.h"

const uint8_t digit_map[10] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

static const struct
{
    char c;
    uint8_t seg;
} letter_map[] = {
    {'A', 0b10001000},
    {'b', 0b10000011},
    {'C', 0b11000110},
    {'E', 0b10000110},
    {'F', 0b10001110},
    {'H', 0b10001001},
    {'n', 0b10101011},
    {'o', 0b10100011},
    {'r', 0b10101111},
    {'U', 0b11000001},
};

static const uint8_t digit_select[4] = {
    0b1000,
    0b0100,
    0b0010,
    0b0001};

static inline void spi_send(uint8_t b)
{
    SPDR = b;
    while (!(SPSR & (1 << SPIF)))
        ;
}

static inline void show_pair(uint8_t seg, uint8_t sel)
{
    PORTB &= ~(1 << LATCH);
    spi_send(0xFF);
    spi_send(0x00);
    PORTB |= (1 << LATCH);

    PORTB &= ~(1 << LATCH);
    spi_send(seg);
    spi_send(sel);
    PORTB |= (1 << LATCH);
}

void display_init(void)
{
    DDRB |= (1 << MOSI) | (1 << SCK) | (1 << LATCH);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void display_number(uint16_t num)
{
    static uint8_t i = 0;
    static uint16_t prev_num = 0xFFFF;
    static uint8_t digs[4];
    static uint8_t first_nonzero = 0;

    if (num != prev_num)
    {
        prev_num = num;
        digs[0] = (num / 1000) % 10;
        digs[1] = (num / 100) % 10;
        digs[2] = (num / 10) % 10;
        digs[3] = num % 10;

        first_nonzero = 0;
        while (first_nonzero < 3 && digs[first_nonzero] == 0)
            first_nonzero++;
        if (num == 0)
            first_nonzero = 3;
    }

    if (i >= first_nonzero)
        show_pair(digit_map[digs[i]], digit_select[i]);
    else
        show_pair(0xFF, digit_select[i]);

    i = (i + 1) % 4;
}

static uint8_t char_to_seg(char c)
{
    if (c >= '0' && c <= '9')
        return digit_map[c - '0'];

    for (uint8_t i = 0; i < sizeof(letter_map) / sizeof(letter_map[0]); i++)
        if (letter_map[i].c == c)
            return letter_map[i].seg;

    return 0xFF;
}

void display_word(const char *text)
{
    static uint8_t i = 0;
    uint8_t len = strlen(text);
    if (len > 4)
        len = 4;

    uint8_t seg = char_to_seg(text[i]);
    show_pair(seg, digit_select[i]);

    i = (i + 1) % len;
}

void display_off(void)
{
    show_pair(0xFF, 0b1111);
}
