#include "i2c_bitbang.h"
#include <util/delay.h>

#define SDA_HIGH() (I2C_DDR &= ~(1 << SDA_PIN))
#define SDA_LOW() (I2C_DDR |= (1 << SDA_PIN))
#define SCL_HIGH() (I2C_DDR &= ~(1 << SCL_PIN))
#define SCL_LOW() (I2C_DDR |= (1 << SCL_PIN))

void I2C_Init(void)
{
    SDA_HIGH();
    SCL_HIGH();
}

void I2C_Start(void)
{
    SDA_HIGH();
    SCL_HIGH();
    _delay_us(5);
    SDA_LOW();
    _delay_us(5);
    SCL_LOW();
}

void I2C_Stop(void)
{
    SDA_LOW();
    _delay_us(4);
    SCL_HIGH();
    _delay_us(4);
    SDA_HIGH();
    _delay_us(4);
}

uint8_t I2C_Write(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        (data & 0x80) ? SDA_HIGH() : SDA_LOW();
        _delay_us(2);
        SCL_HIGH();
        _delay_us(4);
        SCL_LOW();
        data <<= 1;
    }

    SDA_HIGH();
    _delay_us(2);
    SCL_HIGH();
    _delay_us(4);

    uint8_t ack = (I2C_PIN & (1 << SDA_PIN)) ? 1 : 0;

    SCL_LOW();
    _delay_us(4);

    return ack;
}

uint8_t I2C_Read(uint8_t ack)
{
    uint8_t data = 0;

    SDA_HIGH();

    for (uint8_t i = 0; i < 8; i++)
    {
        SCL_LOW();
        _delay_us(4);
        SCL_HIGH();
        _delay_us(4);

        data <<= 1;
        if (I2C_PIN & (1 << SDA_PIN))
            data |= 1;
    }

    if (ack == I2C_ACK)
        SDA_LOW();
    else
        SDA_HIGH();

    SCL_HIGH();
    _delay_us(4);
    SCL_LOW();

    SDA_HIGH();

    return data;
}
