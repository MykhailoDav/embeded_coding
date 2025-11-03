#include "MAX7219Matrix.h"
#include "SimpleSPI.h"
#include "Font8x8.h"
#include "USART.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define GLYPH_WIDTH 8
#define GLYPH_SPACING 1
#define TEXT_GAP 20

void setup();
void loop();
void uart_line_received(uint8_t *buf);
int calcTextWidth(const char *txt);
void drawTextWrapped(int offset);
void updateDisplay(void);
const FontChar *findChar(const char *ch);
void drawTextOnce(int offset);
void display_help_message();
static inline void prompt(void) { printf("$: "); }

char text[128] = "HELLO WORLD";
int textPos = MATRIX_WIDTH;
int textWidth = 0;
volatile bool scrollEnabled = true;
volatile uint16_t scrollDelay = 70;
bool newTextAvailable = false;

void setup()
{
  max7219_init(4, 0);
  max7219_clear();
  
  USART_Init(9600);
  USART_SetStdStreams();
  USART_SetCallback(uart_line_received);
  
  // display_help_message();
  prompt();
  textWidth = calcTextWidth(text) + TEXT_GAP;
}

void loop()
{
  USART_poll();
  updateDisplay();
}

void uart_line_received(uint8_t *buf)
{
  char *cmd = (char *)buf;

  for (char *p = cmd; *p; ++p)
    if (*p == '\r' || *p == '\n')
    {
      *p = 0;
      break;
    }

  if (cmd[0] == '\0')
  {
    prompt();
    return;
  }

  scrollEnabled = false;

  // --- print <text> ---
  if (strncmp(cmd, "print ", 6) == 0)
  {
    const char *msg = cmd + 6;
    size_t len = strlen(msg);

    if (len == 0)
    {
      printf("Syntax error. Use: print <text>\r\n");
    }
    else
    {
      if (len > sizeof(text) - 1)
        len = sizeof(text) - 1;

      strncpy(text, msg, len);
      text[len] = '\0';

      textWidth = calcTextWidth(text) + TEXT_GAP;
      textPos = MATRIX_WIDTH;

      printf("Text set to: %s\r\n", text);
    }
  }
  // --- speed <value> ---
  else if (strncmp(cmd, "speed ", 6) == 0)
  {
    int val = atoi(cmd + 6);
    if (val >= 10 && val <= 500)
    {
      scrollDelay = 500 / val;
      printf("Speed delay set to %d ms\r\n", scrollDelay);
    }
    else
    {
      printf("Speed out of range (10–500)\r\n");
    }
  }
  // --- start ---
  else if (strcmp(cmd, "start") == 0)
  {
    scrollEnabled = true;
    printf("Scrolling started\r\n");
  }
  // --- stop ---
  else if (strcmp(cmd, "stop") == 0)
  {
    scrollEnabled = false;
    printf("Scrolling stopped\r\n");
  }
  // --- clear ---
  else if (strcmp(cmd, "clear") == 0)
  {
    uint8_t sreg = SREG;
    cli();
    max7219_clear();
    sei();
    SREG = sreg;

    matrix_clear_buffer();
    printf("Display cleared\r\n");
  }
  else if (strcmp(cmd, "--help") == 0)
  {
    display_help_message();
  }
  else
  {
    printf("Unknown command: %s\r\n", cmd);
  }

  prompt();

  if (strcmp(cmd, "stop") != 0 && strcmp(cmd, "clear") != 0)
    scrollEnabled = true;
}

void display_help_message()
{
  printf(
      "\r\nShell ready. Commands:\r\n"
      "  print <text> - set new text for scrolling\r\n"
      "  speed N      - set scroll speed (10–500)\r\n"
      "  start        - resume scrolling\r\n"
      "  stop         - pause scrolling\r\n"
      "  clear        - clear display\r\n"
      "  --help       - show help message\r\n"
      "$: ");
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

void delay_ms_runtime(uint16_t ms)
{
  while (ms--)
    _delay_ms(1);
}

void updateDisplay(void)
{
  if (!scrollEnabled)
    return;

  drawTextWrapped(textPos);
  textPos--;

  if (textPos <= -textWidth)
    textPos += textWidth;

  delay_ms_runtime(scrollDelay);
}

int main(void)
{
  setup();
  for (;;)
    loop();
}
