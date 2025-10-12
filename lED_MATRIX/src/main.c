#include <Arduino.h>
#include "MAX7219Matrix.h"
#include "SimpleSPI.h"
#include "Font8x8.h"

#define MATRIX_WIDTH 32
#define CHAR_WIDTH 5
#define CHAR_SPACING 1

// Текст для відображення
const char text[] = "HELLO ARDUINO! 0123456789";
int textPos = MATRIX_WIDTH;

// === Пошук символу в таблиці ===
const FontChar *findChar(const char *ch)
{
  for (uint16_t i = 0; i < Font8x8Count; i++)
  {
    if (strcmp(Font8x8[i].symbol, ch) == 0)
      return &Font8x8[i];
  }
  return NULL;
}

// === Малювання тексту ===
void drawText(int offset)
{
  matrix_clear_buffer();
  int x = offset;

  const char *p = text;
  while (*p)
  {
    char ch[4] = {0};
    uint8_t len = 1;

    // підтримка UTF-8
    if ((uint8_t)*p >= 0xC0)
    {
      if ((p[0] & 0xE0) == 0xC0)
      {
        len = 2;
      }
      else if ((p[0] & 0xF0) == 0xE0)
      {
        len = 3;
      }
    }

    strncpy(ch, p, len);
    p += len;

    const FontChar *fc = findChar(ch);
    if (!fc)
    {
      x += 8 + 1; // пропускаємо, якщо символ не знайдено
      continue;
    }

    // === ОНОВЛЕНА ЧАСТИНА: малюємо символ 8x8 ===
    for (uint8_t col = 0; col < 8; col++)
    {
      uint8_t bits = fc->data[col];
      for (uint8_t y = 0; y < 8; y++)
      {
        bool pixelOn = bits & (1 << y);
        matrix_set_pixel(x + col, y, pixelOn);
      }
    }
    x += 8 + 1; // ширина 8 + відступ 1
  }

  matrix_draw();
}

void setup()
{
  max7219_init(4, 5);
  max7219_clear();
}

void loop()
{
  drawText(textPos);
  delay(1);
  textPos--;

  // Обчислення довжини (5 + 1 піксель відступу)
  int textWidth = strlen(text) * (CHAR_WIDTH + CHAR_SPACING);
  if (textPos < -textWidth)
  {
    textPos = MATRIX_WIDTH;
  }

  delay(70); // швидкість руху
}
