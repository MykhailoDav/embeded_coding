#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include <avr/io.h>

#define SDA_PIN 0
#define SCL_PIN 1

#define I2C_PORT PORTC
#define I2C_PIN PINC
#define I2C_DDR DDRC

#define I2C_ACK 0
#define I2C_NAK 1

#define I2C_WRITE 0
#define I2C_READ 1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);

void I2C_ScanConnectedDevices(void (*lprint)(const char *));

#endif