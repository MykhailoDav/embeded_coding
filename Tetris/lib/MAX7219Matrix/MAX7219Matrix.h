#pragma once
#include <avr/io.h>
#include <stdint.h>

#define MAX7219_CS_DDR   DDRB
#define MAX7219_CS_PORT  PORTB
#define MAX7219_CS_BIT   PB2

#define MODULE_TOP       0
#define MODULE_MID1      1
#define MODULE_MID2      2
#define MODULE_BOTTOM    3


enum {
  MAX7219_NOP         = 0x00,
  MAX7219_DIGIT0      = 0x01,
  MAX7219_DIGIT7      = 0x08,
  MAX7219_DECODE_MODE = 0x09,
  MAX7219_INTENSITY   = 0x0A,
  MAX7219_SCAN_LIMIT  = 0x0B,
  MAX7219_SHUTDOWN    = 0x0C,
  MAX7219_DISPLAYTEST = 0x0F
};

// >>> ВЕРТИКАЛЬ 8×32 <<<
#define MATRIX_WIDTH   8
#define MATRIX_HEIGHT 32

void max7219_init(uint8_t devices, uint8_t intensity);
void max7219_clear(void);
void max7219_set_row_all(uint8_t row1to8, uint8_t pattern);
void max7219_send_to_device(uint8_t idx, uint8_t addr, uint8_t data);

void matrix_clear_buffer(void);
void matrix_set_pixel(uint8_t x, uint8_t y, uint8_t on);
void matrix_draw(void);
