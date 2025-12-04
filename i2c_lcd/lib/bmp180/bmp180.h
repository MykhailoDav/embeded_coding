#ifndef BMP180_H
#define BMP180_H

#include <stdint.h>

#define BMP180_ADDR_WRITE 0xEE
#define BMP180_ADDR_READ  0xEF

uint8_t BMP180_Init(void);

int16_t BMP180_ReadTemperature10x(void);

int32_t BMP180_ReadPressurePa(void);

void LCD_CreateChar(uint8_t location, uint8_t charmap[]);


#endif
