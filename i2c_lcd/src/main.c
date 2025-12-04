#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "USART.h"
#include "i2c.h"
#include "lcd.h"
#include "lcd_chars.h"
#include "bmp180.h"
#include "DS1302.h"
#include "Buttons.h"
#include "UartCommands.h"

#define SEA_LEVEL_PRESSURE 101325  // Тиск на рівні моря в Па
#define BUTTON_PIN PD2  // Кнопка на піні D2

// Режими роботи дисплея
typedef enum {
    MODE_TEMPERATURE,  // Показувати температуру/тиск
    MODE_TIME          // Показувати час
} DisplayMode;

// Глобальні змінні
DisplayMode display_mode = MODE_TEMPERATURE;  // Експортується в UartCommands
uint8_t auto_switch = 1;  // Експортується в UartCommands
static uint32_t last_switch_time = 0;
static bool long_press_handled = false;
static ButtonState button_state = {0};
static ButtonDef button = {&button_state, BUTTON_PIN};
static volatile uint32_t millis_counter = 0;

// ============= Оголошення функцій =============
static void uart_print(const char *s);
static float calculate_altitude(int32_t pressure);
static void show_mode_name(DisplayMode mode);
static void init_timer0(void);
static void init_button(void);
static void init_uart(void);
static void init_i2c_and_lcd(void);
static void init_ds1302(void);
static void init_bmp180(void);
static void handle_button_press(void);
static void handle_auto_switch(void);
static void display_temperature_mode(void);
static void display_time_mode(void);
void setup(void);
void loop(void);
uint32_t millis(void);

// ============= SETUP & LOOP =============

void loop(void)
{
    static uint32_t last_update = 0;
    
    // Обробка UART команд
    USART_poll();
    
    // Обробка кнопки
    handle_button_press();
    
    // Автоматичне перемикання
    handle_auto_switch();
    
    // Оновлення дисплея
    uint32_t current = millis();
    
    // Для температури - оновлюємо кожні 500 мс
    if (display_mode == MODE_TEMPERATURE) {
        if (current - last_update < 500) {
            return;
        }
        last_update = current;
    }
    
    // Відображення в залежності від режиму
    if (display_mode == MODE_TEMPERATURE) {
        display_temperature_mode();
    } else if (display_mode == MODE_TIME) {
        display_time_mode();
    }
}

void setup(void)
{
    init_timer0();
    init_button();
    init_uart();
    init_i2c_and_lcd();
    init_ds1302();
    init_bmp180();
    
    uart_print("System initialized!\r\n");
}

// ============= ПЕРЕРИВАННЯ =============

ISR(TIMER0_COMPA_vect) {
    millis_counter++;
}

uint32_t millis(void) {
    return millis_counter;
}

// ============= УТИЛІТНІ ФУНКЦІЇ =============

static void uart_print(const char *s)
{
    while (*s)
        USART_PutChar(*s++);
}

static float calculate_altitude(int32_t pressure)
{
    // Барометрична формула: h = 44330 * (1 - (P/P0)^0.1903)
    float ratio = (float)pressure / SEA_LEVEL_PRESSURE;
    float altitude = 44330.0 * (1.0 - pow(ratio, 0.1903));
    return altitude;
}

static void show_mode_name(DisplayMode mode) {
    LCD_Clear();
    if (mode == MODE_TEMPERATURE) {
        LCD_Print(0, 0, "  TEMPERATURE   ");
        LCD_Print(0, 1, "     MODE       ");
    } else if (mode == MODE_TIME) {
        LCD_Print(0, 0, "      TIME      ");
        LCD_Print(0, 1, "      MODE      ");
    }
    _delay_ms(1000);
    LCD_Clear();
}

// ============= ІНІЦІАЛІЗАЦІЯ =============

static void init_timer0(void)
{
    // Timer0 для millis() - переривання кожну 1 мс
    TCCR0A = (1 << WGM01);  // CTC режим
    TCCR0B = (1 << CS01) | (1 << CS00);  // Prescaler 64
    OCR0A = 249;  // 250 тактів = 1 мс
    TIMSK0 = (1 << OCIE0A);  // Увімкнути переривання
    sei();  // Глобально увімкнути переривання
}

static void init_button(void)
{
    DDRD &= ~(1 << BUTTON_PIN);  // Пін як вхід
    PORTD |= (1 << BUTTON_PIN);  // Увімкнути pull-up резистор
    Buttons_Init(&button, 1);
    uart_print("Button initialized on PD2\r\n");
}

static void init_uart(void)
{
    USART_Init(9600);
    USART_SetStdStreams();
    USART_SetCallback(UartCommands_Handler);
    uart_print("Start\r\n");
    uart_print("Type 'help' for commands\r\n");
}

static void init_i2c_and_lcd(void)
{
    I2C_Init();
    uart_print("I2C init OK\r\n");
    uart_print("Scanning I2C devices...\r\n");
    // I2C_ScanConnectedDevices(uart_print);

    uart_print("\r\nInitializing LCD...\r\n");
    if (LCD_Init(LCD_ADDR) != 0) {
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
}

static void init_ds1302(void)
{
    uart_print("Initializing DS1302...\r\n");
    DS1302_Init();
    // Встановити початковий час (розкоментуйте для першого запуску)
    // DS1302_Time init_time = {0, 30, 15, 4, 12, 3, 2025};
    // DS1302_SetTime(&init_time);
    uart_print("DS1302 OK\r\n");
}

static void init_bmp180(void)
{
    if (BMP180_Init() != 0) {
        uart_print("BMP180 NOT FOUND!\r\n");
        LCD_Clear();
        LCD_Print(0, 0, "BMP180 ERROR");
        while (1) { USART_poll(); }
    }
    uart_print("BMP180 OK\r\n");
}

// ============= ОБРОБКА ПОДІЙ =============

static void handle_button_press(void)
{
    Buttons_Update(millis());
    
    // Довге натискання (2 сек) - автоматичний режим
    if (button_state.long_press) {
        auto_switch = 1;
        long_press_handled = true;
        LCD_Clear();
        LCD_Print(0, 0, "   AUTOMATIC   ");
        LCD_Print(0, 1, "     MODE      ");
        uart_print("Mode: AUTO SWITCH\r\n");
        _delay_ms(1000);
        LCD_Clear();
        last_switch_time = millis();
        return;
    }
    
    // Коротке натискання - ручне перемикання
    if (button_state.released) {
        if (long_press_handled) {
            long_press_handled = false;
            return;
        }
        
        auto_switch = 0;
        
        if (display_mode == MODE_TEMPERATURE) {
            display_mode = MODE_TIME;
        } else {
            display_mode = MODE_TEMPERATURE;
        }
        
        show_mode_name(display_mode);
        uart_print("Mode switched manually\r\n");
    }
}

static void handle_auto_switch(void)
{
    if (!auto_switch) return;
    
    uint32_t current_time = millis();
    if (current_time - last_switch_time >= 2000) {
        last_switch_time = current_time;
        
        if (display_mode == MODE_TEMPERATURE) {
            display_mode = MODE_TIME;
        } else {
            display_mode = MODE_TEMPERATURE;
        }
        LCD_Clear();
    }
}

// ============= ВІДОБРАЖЕННЯ ДАНИХ =============

static void display_temperature_mode(void)
{
    int16_t  t10 = BMP180_ReadTemperature10x();
    int32_t  p   = BMP180_ReadPressurePa();
    float    alt = calculate_altitude(p);

    int16_t t_int = t10 / 10;
    int16_t t_frac = t10 % 10;
    if (t_frac < 0) t_frac = -t_frac;

    char buf[17];
    LCD_SetXY(0, 0);
    Lcd_SendChar(3);  // Термометр
    snprintf(buf, sizeof(buf), "%2d.%1dC H:%dm", t_int, t_frac, (int)alt);
    LCD_Print(1, 0, buf);

    snprintf(buf, sizeof(buf), "P:%ld Pa", p);
    LCD_Print(0, 1, buf);
}

static void display_time_mode(void)
{
    DS1302_Time time;
    DS1302_GetTime(&time);
    
    const char* days[] = {"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    
    char buf[17];
    snprintf(buf, sizeof(buf), "%02d.%02d.%04d %s", 
             time.date, time.month, time.year, 
             (time.day >= 1 && time.day <= 7) ? days[time.day] : "");
    LCD_Print(0, 0, buf);
    
    snprintf(buf, sizeof(buf), "    %02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    LCD_Print(0, 1, buf);
}

int main(void)
{
    setup();
    
    while (1)
    {
        loop();
    }
}
