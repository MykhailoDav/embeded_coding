# DS1302 RTC Library

Library for working with DS1302 Real-Time Clock module.

## Connection

| DS1302 | ATmega328P | Arduino |
|--------|------------|---------|
| VCC    | +5V        | 5V      |
| GND    | GND        | GND     |
| CLK    | PD5        | D5      |
| DAT    | PD6        | D6      |
| RST    | PD4        | D4      |

## Usage

### Initialization
```c
#include "DS1302.h"

DS1302_Init();
```

### Setting Time
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

### Reading Time
```c
DS1302_Time time;
DS1302_GetTime(&time);

printf("Time: %02d:%02d:%02d\n", time.hours, time.minutes, time.seconds);
printf("Date: %02d.%02d.%04d\n", time.date, time.month, time.year);
```

## UART Commands

- `time` - show time only
- `temp` - show temperature only
- `auto` - automatic switching (every 2 sec)
- `settime HH:MM:SS` - set time
- `setdate DD.MM.YYYY` - set date
- `help` - show help

## Button Control

- **Short press** - manual switching between modes (temperature ↔ time)
- **Long press (2 sec)** - enable automatic mode (switching every 2 seconds)

## Example

```c
void main(void) {
    DS1302_Init();
    
    // Set time on first run
    DS1302_Time init_time = {0, 30, 15, 4, 12, 3, 2025};
    DS1302_SetTime(&init_time);
    
    while(1) {
        DS1302_Time current;
        DS1302_GetTime(&current);
        
        // Use time
        _delay_ms(1000);
    }
}
```
