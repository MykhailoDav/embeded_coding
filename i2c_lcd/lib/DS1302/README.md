# DS1302 RTC Бібліотека

Бібліотека для роботи з модулем Real-Time Clock DS1302.

## Підключення

| DS1302 | ATmega328P | Arduino |
|--------|------------|---------|
| VCC    | +5V        | 5V      |
| GND    | GND        | GND     |
| CLK    | PD5        | D5      |
| DAT    | PD6        | D6      |
| RST    | PD4        | D4      |

## Використання

### Ініціалізація
```c
#include "DS1302.h"

DS1302_Init();
```

### Встановлення часу
```c
DS1302_Time time = {
    .seconds = 0,
    .minutes = 30,
    .hours = 15,
    .date = 4,
    .month = 12,
    .day = 3,      // 1=Monday, 7=Sunday
    .year = 2025
};

DS1302_SetTime(&time);
```

### Читання часу
```c
DS1302_Time time;
DS1302_GetTime(&time);

printf("Time: %02d:%02d:%02d\n", time.hours, time.minutes, time.seconds);
printf("Date: %02d.%02d.%04d\n", time.date, time.month, time.year);
```

## UART Команди

- `time` - показати тільки час
- `temp` - показати тільки температуру
- `auto` - автоматичне перемикання (кожні 2 сек)
- `settime HH:MM:SS` - встановити час
- `setdate DD.MM.YYYY` - встановити дату
- `help` - показати допомогу

## Керування кнопкою

- **Коротке натискання** - ручне перемикання між режимами (температура ↔ час)
- **Довге натискання (2 сек)** - увімкнути автоматичний режим (перемикання кожні 2 секунди)

## Приклад

```c
void main(void) {
    DS1302_Init();
    
    // Встановити час при першому запуску
    DS1302_Time init_time = {0, 30, 15, 4, 12, 3, 2025};
    DS1302_SetTime(&init_time);
    
    while(1) {
        DS1302_Time current;
        DS1302_GetTime(&current);
        
        // Використовувати час
        _delay_ms(1000);
    }
}
```
