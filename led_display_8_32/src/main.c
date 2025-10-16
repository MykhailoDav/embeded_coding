#include "MAX7219Matrix.h"
#include "SimpleSPI.h"
#include "Font8x8.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define GLYPH_WIDTH 8
#define GLYPH_SPACING 1
#define TEXT_GAP 20

const FontChar *findChar(const char *ch);
int calcTextWidth(const char *txt);
void drawTextOnce(int offset);
void drawTextWrapped(int offset);
void setup(void);
void loop(void);

const char text[] = "DAVUDENKO MYKHAILO IK-31";
int textPos = MATRIX_WIDTH;
int textWidth = 0;

void setup(void)
{
  max7219_init(4, 0);
  max7219_clear();

  textWidth = calcTextWidth(text) + TEXT_GAP;
  textPos = MATRIX_WIDTH;
}

void loop(void)
{
  drawTextWrapped(textPos);
  textPos--;

  if (textPos <= -textWidth)
    textPos += textWidth;

  _delay_ms(70);
}

int main(void)
{
  setup();
  for (;;)
    loop();
}

const FontChar *findChar(const char *ch)
{
  for (uint16_t i = 0; i < Font8x8Count; i++)
  {
    if (strcmp(Font8x8[i].symbol, ch) == 0)
      return &Font8x8[i];
  }
  return NULL;
}

int calcTextWidth(const char *txt)
{
  int w = 0;
  const char *p = txt;
  while (*p)
  {
    uint8_t len = 1;
    if ((uint8_t)*p >= 0xC0)
    {
      if ((p[0] & 0xE0) == 0xC0)
        len = 2;
      else if ((p[0] & 0xF0) == 0xE0)
        len = 3;
    }
    p += len;
    w += GLYPH_WIDTH + GLYPH_SPACING;
  }
  return w;
}

void drawTextOnce(int offset)
{
  int x = offset;
  const char *p = text;

  while (*p)
  {
    char ch[4] = {0};
    uint8_t len = 1;

    if ((uint8_t)*p >= 0xC0)
    {
      if ((p[0] & 0xE0) == 0xC0)
        len = 2;
      else if ((p[0] & 0xF0) == 0xE0)
        len = 3;
    }

    strncpy(ch, p, len);
    p += len;

    const FontChar *fc = findChar(ch);
    if (!fc)
    {
      x += GLYPH_WIDTH + GLYPH_SPACING;
      continue;
    }

    for (uint8_t col = 0; col < GLYPH_WIDTH; col++)
    {
      uint8_t bits = fc->data[col];
      int drawX = x + col;

      if (drawX >= 0 && drawX < MATRIX_WIDTH)
      {
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
        {
          bool pixelOn = bits & (1 << y);
          matrix_set_pixel(drawX, y, pixelOn);
        }
      }
    }

    x += GLYPH_WIDTH + GLYPH_SPACING;
  }
}

void drawTextWrapped(int offset)
{
  matrix_clear_buffer();
  drawTextOnce(offset);
  drawTextOnce(offset + textWidth);
  matrix_draw();
}
