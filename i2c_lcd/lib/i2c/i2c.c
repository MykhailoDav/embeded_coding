#include "i2c.h"
#include <util/delay.h>
#include <stdio.h>

#define SDA_HIGH() I2C_DDR &= ~(1<<SDA_PIN)
#define SDA_LOW() I2C_DDR |= (1<<SDA_PIN)
#define SCL_HIGH() I2C_DDR &= ~(1<<SCL_PIN)
#define SCL_LOW() I2C_DDR |= (1<<SCL_PIN)

void I2C_Init(void)
{
   // SCL = 1, SDA = 1
   I2C_DDR &= ~((1<<SDA_PIN)|(1<<SCL_PIN)); 
   I2C_PORT &= ~((1<<SDA_PIN)|(1<<SCL_PIN));
}

//-----------------------------------------------
void I2C_Start(void)
{
     // SCL = 1, SDA = 1
     I2C_DDR &= ~((1<<SCL_PIN)|(1<<SDA_PIN));  
     _delay_us(5); // HDEL
     SDA_LOW();    // sda = 0
     _delay_us(5); // HDEL
     SCL_LOW();    //??? scl = 0
}

//-----------------------------------------------------------------------------
void I2C_Stop(void)
{
     SDA_LOW();
     _delay_us(5); // HDEL
     SCL_HIGH();   // scl = 1;
     _delay_us(3); // QDEL
     SDA_HIGH();   // sda = 1;
     _delay_us(2);
}

//-----------------------------------------------------------------------------
uint8_t I2C_Write(uint8_t data)
{
  uint8_t i, ack;
    for (i = 0; i < 8; i++)
    {
        SCL_LOW();
        _delay_us(2);
        if (data & 0x80) {
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        _delay_us(4);
        SCL_HIGH();
        _delay_us(5);
        data <<= 1;
    }
    SCL_LOW();
    _delay_us(3);
    SDA_HIGH();
    _delay_us(5);
    SCL_HIGH();
    _delay_us(3);
    ack = I2C_PIN & (1<<SDA_PIN);
    _delay_us(2);
    SCL_LOW();
    _delay_us(5);
 return ack;
}

//-----------------------------------------------------------------------------
uint8_t I2C_Read(uint8_t ack)
{
  uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL_LOW(); 
        _delay_us(5);
        SCL_HIGH(); 
        _delay_us(5); //while((I2C_PIN&(1<<SDA_PIN))==0){;}
        if (I2C_PIN & (1<<SDA_PIN)) {
            data |= 1;
        }
    }
    SCL_LOW();
    _delay_us(2);
    if (!ack) {
        SDA_LOW();
    } else {
        SDA_HIGH();
    }
    _delay_us(3);
    SCL_HIGH();
    _delay_us(5);
    SCL_LOW();
    _delay_us(5);
  return data;
}

//-----------------------------------------------------------------------------
void I2C_ScanConnectedDevices(void (*lprint)(const char *))
{
  char buf[8];
  uint8_t res;

    lprint("\n     ");
	for(int i = 0; i < 16; i++)
	{
		sprintf(buf, "0%X|", i);
		lprint(buf);
	}

	lprint("\n    ------------------------------------------------");
	lprint("\n 00|                        ");
	for(int i= 8; i < 240; i++)
	{
	    if(i % 16 == 0) {
		    sprintf(buf, "\n %X0|", i/16);
			lprint(buf);
	    }

        if((i & 1) == 0) {
            I2C_Start();
            res = I2C_Write(i);
            I2C_Stop();

		    if(res == I2C_ACK) {
			    sprintf(buf, " %X", i);
			    lprint(buf);
		    } else lprint(" --");
	    } else lprint(" --");   
    }
	lprint("\n\n");
}