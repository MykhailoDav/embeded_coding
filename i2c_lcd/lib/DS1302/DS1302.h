#ifndef DS1302_H
#define DS1302_H

#include <stdint.h>

// Структура для зберігання часу
typedef struct {
    uint8_t seconds;   // 0-59
    uint8_t minutes;   // 0-59
    uint8_t hours;     // 0-23
    uint8_t date;      // 1-31
    uint8_t month;     // 1-12
    uint8_t day;       // 1-7 (1=Monday)
    uint16_t year;     // 2000-2099
} DS1302_Time;

// Ініціалізація DS1302
void DS1302_Init(void);

// Встановити час
void DS1302_SetTime(DS1302_Time *time);

// Прочитати час
void DS1302_GetTime(DS1302_Time *time);

// Запустити/зупинити годинник
void DS1302_Start(void);
void DS1302_Halt(void);

// Захист від запису
void DS1302_WriteProtect(uint8_t enable);

#endif
