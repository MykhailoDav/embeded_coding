#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>

#include "USART.h"
#include "i2c.h"
#include "lcd.h"
#include "lcd_chars.h"
#include "bmp180.h"

#define SEA_LEVEL_PRESSURE 101325  // Тиск на рівні моря в Па

void uart_print(const char *s)
{
    while (*s)
        USART_PutChar(*s++);
}

float calculate_altitude(int32_t pressure)
{
    // Барометрична формула: h = 44330 * (1 - (P/P0)^0.1903)
    float ratio = (float)pressure / SEA_LEVEL_PRESSURE;
    float altitude = 44330.0 * (1.0 - pow(ratio, 0.1903));
    return altitude;
}

void setup(void)
{
    // --- UART ---
    USART_Init(9600);
    USART_SetStdStreams();
    uart_print("Start\r\n");

    // --- I2C ---
    I2C_Init();
    uart_print("I2C init OK\r\n");

    uart_print("Scanning I2C devices...\r\n");
    I2C_ScanConnectedDevices(uart_print);

    // --- LCD ---
    uart_print("\r\nInitializing LCD...\r\n");
    if (LCD_Init(LCD_ADDR) != 0)
    {
        uart_print("LCD NOT FOUND!\r\n");
        while (1) { USART_poll(); }
    }
    uart_print("LCD OK\r\n");

    Lcd_BackLightOnOff(1);

    // Створюємо custom символи
    LCD_CreateChar(0, smiley_happy);
    LCD_CreateChar(1, smiley_sad);
    LCD_CreateChar(2, heart);
    LCD_CreateChar(3, thermometer);

    LCD_Clear();
    LCD_Print(0, 0, "Welcome! ");
    LCD_SetXY(9, 0);
    Lcd_SendChar(0);
    LCD_SetXY(10, 0);
    Lcd_SendChar(2);
    LCD_Print(0, 1, "Weather Station");

    _delay_ms(3000);

    // --- BMP180 ---
    if (BMP180_Init() != 0)
    {
        uart_print("BMP180 NOT FOUND!\r\n");
        LCD_Clear();
        LCD_Print(0, 0, "BMP180 ERROR");
        while (1) { USART_poll(); }
    }
    uart_print("BMP180 OK\r\n");

    LCD_Clear();
}

void loop(void)
{
    char buf[17];
    
    int16_t  t10 = BMP180_ReadTemperature10x();  // в 0.1°C
    int32_t  p   = BMP180_ReadPressurePa();      // в Па
    float    alt = calculate_altitude(p);        // висота в метрах

    int16_t t_int = t10 / 10;
    int16_t t_frac = t10 % 10;
    if (t_frac < 0) t_frac = -t_frac;

    LCD_SetXY(0, 0);
    Lcd_SendChar(3);
    snprintf(buf, sizeof(buf), "%2d.%1dC H:%dm", t_int, t_frac, (int)alt);
    LCD_Print(1, 0, buf);

    snprintf(buf, sizeof(buf), "P:%ld Pa", p);
    LCD_Print(0, 1, buf);

    printf("T = %d.%d C, P = %ld Pa, Alt = %.1i m\r\n", t_int, t_frac, p, (int)alt);

    _delay_ms(500);
    USART_poll();
}

int main(void)
{
    setup();
    
    while (1)
    {
        loop();
    }
}
