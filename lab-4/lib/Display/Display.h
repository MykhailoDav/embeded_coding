#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <stdint.h>

#define LATCH PB2
#define MOSI  PB3
#define SCK   PB5

extern const uint8_t digit_map[10];

void display_init(void);

void display_number(uint16_t num);

void display_word(const char *text);

void display_off(void);

#endif
