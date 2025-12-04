#include "UartCommands.h"
#include "USART.h"
#include "lcd.h"
#include "DS1302.h"
#include <string.h>
#include <stdio.h>

// Зовнішні змінні з main.c (тип int бо enum)
extern int display_mode;
extern uint8_t auto_switch;

static void uart_print(const char *s)
{
    while (*s)
        USART_PutChar(*s++);
}

void UartCommands_Handler(uint8_t *cmd)
{
    char *command = (char *)cmd;
    
    // Команда для режиму температури
    if (strcmp(command, "temp") == 0 || strcmp(command, "TEMP") == 0) {
        display_mode = 0;  // MODE_TEMPERATURE
        auto_switch = 0;
        LCD_Clear();
        uart_print("Mode: TEMPERATURE\r\n");
        return;
    }
    
    // Команда для режиму часу
    if (strcmp(command, "time") == 0 || strcmp(command, "TIME") == 0) {
        display_mode = 1;  // MODE_TIME
        auto_switch = 0;
        LCD_Clear();
        uart_print("Mode: TIME\r\n");
        return;
    }
    
    // Команда для автоперемикання
    if (strcmp(command, "auto") == 0 || strcmp(command, "AUTO") == 0) {
        auto_switch = 1;
        uart_print("Mode: AUTO SWITCH\r\n");
        return;
    }
    
    // Команда для встановлення часу: "settime HH:MM:SS"
    if (strncmp(command, "settime ", 8) == 0) {
        DS1302_Time new_time;
        DS1302_GetTime(&new_time);  // Зберегти дату
        
        int h, m, s;
        if (sscanf(&command[8], "%d:%d:%d", &h, &m, &s) == 3) {
            new_time.hours = h;
            new_time.minutes = m;
            new_time.seconds = s;
            DS1302_SetTime(&new_time);
            uart_print("Time set: ");
            char buf[16];
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d\r\n", h, m, s);
            uart_print(buf);
        } else {
            uart_print("Format: settime HH:MM:SS\r\n");
        }
        return;
    }
    
    // Команда для встановлення дати: "setdate DD.MM.YYYY"
    if (strncmp(command, "setdate ", 8) == 0) {
        DS1302_Time new_time;
        DS1302_GetTime(&new_time);  // Зберегти час
        
        int d, m, y;
        if (sscanf(&command[8], "%d.%d.%d", &d, &m, &y) == 3) {
            new_time.date = d;
            new_time.month = m;
            new_time.year = y;
            DS1302_SetTime(&new_time);
            char buf[32];
            snprintf(buf, sizeof(buf), "Date set: %02d.%02d.%04d\r\n", d, m, y);
            uart_print(buf);
        } else {
            uart_print("Format: setdate DD.MM.YYYY\r\n");
        }
        return;
    }
    
    // Команда допомоги
    if (strcmp(command, "help") == 0 || strcmp(command, "HELP") == 0) {
        uart_print("\r\n=== LCD UART Control ===\r\n");
        uart_print("temp              - show temperature\r\n");
        uart_print("time              - show time\r\n");
        uart_print("auto              - auto switch mode\r\n");
        uart_print("settime HH:MM:SS  - set time\r\n");
        uart_print("setdate DD.MM.YYYY- set date\r\n");
        uart_print("help              - show this help\r\n");
        uart_print("========================\r\n");
        return;
    }
}
