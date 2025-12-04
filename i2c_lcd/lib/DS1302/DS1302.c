#include "DS1302.h"
#include <avr/io.h>
#include <util/delay.h>

// Налаштування пінів (можна змінити під ваше підключення)
#define DS1302_PORT     PORTD
#define DS1302_DDR      DDRD
#define DS1302_PIN      PIND

#define DS1302_RST      PD4    // RST/CE пін
#define DS1302_CLK      PD5    // CLK пін
#define DS1302_DAT      PD6    // DAT пін

// Команди DS1302
#define DS1302_WRITE_PROTECT    0x8E
#define DS1302_CLOCK_HALT       0x80
#define DS1302_SECONDS          0x80
#define DS1302_MINUTES          0x82
#define DS1302_HOURS            0x84
#define DS1302_DATE             0x86
#define DS1302_MONTH            0x88
#define DS1302_DAY              0x8A
#define DS1302_YEAR             0x8C

// Макроси для керування пінами
#define RST_LOW()   DS1302_PORT &= ~(1 << DS1302_RST)
#define RST_HIGH()  DS1302_PORT |= (1 << DS1302_RST)
#define CLK_LOW()   DS1302_PORT &= ~(1 << DS1302_CLK)
#define CLK_HIGH()  DS1302_PORT |= (1 << DS1302_CLK)
#define DAT_LOW()   DS1302_PORT &= ~(1 << DS1302_DAT)
#define DAT_HIGH()  DS1302_PORT |= (1 << DS1302_DAT)
#define DAT_READ()  (DS1302_PIN & (1 << DS1302_DAT))

#define DAT_OUTPUT() DS1302_DDR |= (1 << DS1302_DAT)
#define DAT_INPUT()  DS1302_DDR &= ~(1 << DS1302_DAT)

// Конвертація BCD <-> Decimal
static uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static uint8_t dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

// Затримка для синхронізації
static void ds1302_delay(void) {
    _delay_us(2);
}

// Запис одного байта в DS1302
static void ds1302_write_byte(uint8_t byte) {
    DAT_OUTPUT();
    
    for (uint8_t i = 0; i < 8; i++) {
        CLK_LOW();
        
        if (byte & 0x01) {
            DAT_HIGH();
        } else {
            DAT_LOW();
        }
        
        ds1302_delay();
        CLK_HIGH();
        ds1302_delay();
        
        byte >>= 1;
    }
}

// Читання одного байта з DS1302
static uint8_t ds1302_read_byte(void) {
    uint8_t byte = 0;
    
    DAT_INPUT();
    
    for (uint8_t i = 0; i < 8; i++) {
        CLK_LOW();
        ds1302_delay();
        
        if (DAT_READ()) {
            byte |= (1 << i);
        }
        
        CLK_HIGH();
        ds1302_delay();
    }
    
    return byte;
}

// Запис регістра
static void ds1302_write_register(uint8_t reg, uint8_t data) {
    RST_LOW();
    CLK_LOW();
    ds1302_delay();
    
    RST_HIGH();
    ds1302_delay();
    
    ds1302_write_byte(reg);      // Команда (адреса для запису)
    ds1302_write_byte(data);     // Дані
    
    RST_LOW();
    ds1302_delay();
}

// Читання регістра
static uint8_t ds1302_read_register(uint8_t reg) {
    uint8_t data;
    
    RST_LOW();
    CLK_LOW();
    ds1302_delay();
    
    RST_HIGH();
    ds1302_delay();
    
    ds1302_write_byte(reg | 0x01);  // Додаємо біт читання
    data = ds1302_read_byte();
    
    RST_LOW();
    ds1302_delay();
    
    return data;
}

// Ініціалізація DS1302
void DS1302_Init(void) {
    // Налаштувати піни як виходи
    DS1302_DDR |= (1 << DS1302_RST) | (1 << DS1302_CLK);
    DAT_OUTPUT();
    
    // Встановити початковий стан
    RST_LOW();
    CLK_LOW();
    DAT_LOW();
    
    _delay_ms(10);
    
    // Зняти захист запису
    DS1302_WriteProtect(0);
    
    // Запустити годинник
    DS1302_Start();
}

// Встановити час
void DS1302_SetTime(DS1302_Time *time) {
    DS1302_WriteProtect(0);
    
    ds1302_write_register(DS1302_SECONDS, dec_to_bcd(time->seconds));
    ds1302_write_register(DS1302_MINUTES, dec_to_bcd(time->minutes));
    ds1302_write_register(DS1302_HOURS, dec_to_bcd(time->hours));
    ds1302_write_register(DS1302_DATE, dec_to_bcd(time->date));
    ds1302_write_register(DS1302_MONTH, dec_to_bcd(time->month));
    ds1302_write_register(DS1302_DAY, dec_to_bcd(time->day));
    ds1302_write_register(DS1302_YEAR, dec_to_bcd(time->year - 2000));
    
    DS1302_WriteProtect(1);
}

// Прочитати час
void DS1302_GetTime(DS1302_Time *time) {
    uint8_t data;
    
    data = ds1302_read_register(DS1302_SECONDS);
    time->seconds = bcd_to_dec(data & 0x7F);  // Маска біта CH
    
    time->minutes = bcd_to_dec(ds1302_read_register(DS1302_MINUTES));
    time->hours = bcd_to_dec(ds1302_read_register(DS1302_HOURS) & 0x3F);  // 24-годинний формат
    time->date = bcd_to_dec(ds1302_read_register(DS1302_DATE));
    time->month = bcd_to_dec(ds1302_read_register(DS1302_MONTH));
    time->day = bcd_to_dec(ds1302_read_register(DS1302_DAY));
    time->year = bcd_to_dec(ds1302_read_register(DS1302_YEAR)) + 2000;
}

// Запустити годинник
void DS1302_Start(void) {
    uint8_t seconds = ds1302_read_register(DS1302_SECONDS);
    ds1302_write_register(DS1302_SECONDS, seconds & 0x7F);  // Очистити біт CH
}

// Зупинити годинник
void DS1302_Halt(void) {
    uint8_t seconds = ds1302_read_register(DS1302_SECONDS);
    ds1302_write_register(DS1302_SECONDS, seconds | 0x80);  // Встановити біт CH
}

// Захист від запису
void DS1302_WriteProtect(uint8_t enable) {
    if (enable) {
        ds1302_write_register(DS1302_WRITE_PROTECT, 0x80);
    } else {
        ds1302_write_register(DS1302_WRITE_PROTECT, 0x00);
    }
}
