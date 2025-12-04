#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "i2c.h"
#include "bmp180.h"

#define BMP180_REG_CALIB_START  0xAA
#define BMP180_REG_CHIP_ID      0xD0
#define BMP180_REG_CONTROL      0xF4
#define BMP180_REG_OUT_MSB      0xF6

#define BMP180_CMD_TEMP         0x2E
#define BMP180_CMD_PRESSURE     0x34

#define BMP180_OSS              1

static int16_t  AC1, AC2, AC3, B1, B2, MB, MC, MD;
static uint16_t AC4, AC5, AC6;
static int32_t  B5;

static void bmp180_write_reg(uint8_t reg, uint8_t value)
{
    I2C_Start();
    I2C_Write(BMP180_ADDR_WRITE);
    I2C_Write(reg);
    I2C_Write(value);
    I2C_Stop();
}

static uint8_t bmp180_read8(uint8_t reg)
{
    uint8_t val;

    I2C_Start();
    I2C_Write(BMP180_ADDR_WRITE);
    I2C_Write(reg);

    I2C_Start();
    I2C_Write(BMP180_ADDR_READ);
    val = I2C_Read(1);
    I2C_Stop();

    return val;
}

static uint16_t bmp180_read16(uint8_t reg)
{
    uint8_t msb, lsb;
    uint16_t val;

    I2C_Start();
    I2C_Write(BMP180_ADDR_WRITE);
    I2C_Write(reg);

    I2C_Start();
    I2C_Write(BMP180_ADDR_READ);
    msb = I2C_Read(0); // ACK
    lsb = I2C_Read(1); // NAK
    I2C_Stop();

    val = ((uint16_t)msb << 8) | lsb;
    return val;
}

static void bmp180_read_calibration(void)
{
    AC1 = (int16_t) bmp180_read16(0xAA);
    AC2 = (int16_t) bmp180_read16(0xAC);
    AC3 = (int16_t) bmp180_read16(0xAE);
    AC4 =          bmp180_read16(0xB0);
    AC5 =          bmp180_read16(0xB2);
    AC6 =          bmp180_read16(0xB4);
    B1  = (int16_t) bmp180_read16(0xB6);
    B2  = (int16_t) bmp180_read16(0xB8);
    MB  = (int16_t) bmp180_read16(0xBA);
    MC  = (int16_t) bmp180_read16(0xBC);
    MD  = (int16_t) bmp180_read16(0xBE);
}

uint8_t BMP180_Init(void)
{
    uint8_t id = bmp180_read8(BMP180_REG_CHIP_ID);
    if (id != 0x55)
    {
        return 1;
    }

    bmp180_read_calibration();
    return 0;
}

int16_t BMP180_ReadTemperature10x(void)
{
    int32_t UT, X1, X2;
    int16_t T;

    bmp180_write_reg(BMP180_REG_CONTROL, BMP180_CMD_TEMP);
    _delay_ms(5); 

    UT = bmp180_read16(BMP180_REG_OUT_MSB);

    X1 = ((UT - (int32_t)AC6) * (int32_t)AC5) >> 15;
    X2 = ((int32_t)MC << 11) / (X1 + MD);
    B5 = X1 + X2;
    T  = (int16_t)((B5 + 8) >> 4); // 0.1°C

    return T;
}

int32_t BMP180_ReadPressurePa(void)
{
    int32_t UP;
    int32_t B6, X1, X2, X3;
    int32_t B3;
    uint32_t B4, B7;
    int32_t p;

    uint8_t msb, lsb, xlsb;

    BMP180_ReadTemperature10x();

    bmp180_write_reg(BMP180_REG_CONTROL, BMP180_CMD_PRESSURE + (BMP180_OSS << 6));
    _delay_ms(8); 

    I2C_Start();
    I2C_Write(BMP180_ADDR_WRITE);
    I2C_Write(BMP180_REG_OUT_MSB);

    I2C_Start();
    I2C_Write(BMP180_ADDR_READ);
    msb  = I2C_Read(0); // ACK
    lsb  = I2C_Read(0); // ACK
    xlsb = I2C_Read(1); // NAK
    I2C_Stop();

    UP = (((int32_t)msb << 16) | ((int32_t)lsb << 8) | xlsb) >> (8 - BMP180_OSS);

    B6 = B5 - 4000;
    X1 = ((int32_t)B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t)AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)AC1 * 4 + X3) << BMP180_OSS) + 2) / 4;

    X1 = ((int32_t)AC3 * B6) >> 13;
    X2 = ((int32_t)B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)AC4 * (uint32_t)(X3 + 32768)) >> 15;

    B7 = ((uint32_t)UP - (uint32_t)B3) * (50000UL >> BMP180_OSS);

    if (B7 < 0x80000000UL)
        p = (int32_t)((B7 * 2) / B4);
    else
        p = (int32_t)((B7 / B4) * 2);

    X1 = (p >> 8);
    X1 = (X1 * X1 * 3038L) >> 16;
    X2 = (-7357L * p) >> 16;
    p = p + ((X1 + X2 + 3791L) >> 4);

    return p; // Pa
}
