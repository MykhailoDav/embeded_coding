#ifndef I2C_BITBANG_H
#define I2C_BITBANG_H

#include <avr/io.h>

#define SDA_PIN PC4
#define SCL_PIN PC5

#define I2C_DDR DDRC
#define I2C_PORT PORTC
#define I2C_PIN PINC

#define I2C_ACK 0
#define I2C_NACK 1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);

#endif
