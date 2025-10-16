#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "MatrixRain.h"
#include "MAX7219Matrix.h"

#define WIDTH 8
#define HEIGHT 32
#define DROPS 12
#define RAIN_DELAY 60
#define RAIN_FRAMES 200
#define MIN_LEN 3
#define MAX_LEN 7

typedef struct
{
    int y;
    int length;
    uint8_t active;
    uint8_t x;
} Drop;

static Drop rain[DROPS];

void MatrixRain_Init(void)
{
    for (uint8_t i = 0; i < DROPS; i++)
    {
        rain[i].x = rand() % WIDTH;
        rain[i].y = -(rand() % HEIGHT);
        rain[i].length = MIN_LEN + rand() % (MAX_LEN - MIN_LEN + 1);
        rain[i].active = 1;
    }
}

void MatrixRain_DrawFrame(void)
{
    matrix_clear_buffer();

    for (uint8_t i = 0; i < DROPS; i++)
    {
        if (!rain[i].active)
        {
            continue;
        }
        for (int j = 0; j < rain[i].length; j++)
        {
            int y = rain[i].y - j;
            if (y >= 0 && y < HEIGHT)
            {
                matrix_set_pixel(rain[i].x, y, 1);
            }
        }

        // move down
        rain[i].y++;

        if (rain[i].y - rain[i].length > HEIGHT)
        {
            rain[i].x = rand() % WIDTH;
            rain[i].y = -(rand() % HEIGHT);
            rain[i].length = MIN_LEN + rand() % (MAX_LEN - MIN_LEN + 1);
        }
    }

    matrix_draw();
}

void MatrixRain_StartAnimation(void)
{
    MatrixRain_Init();

    for (uint16_t frame = 0; frame < RAIN_FRAMES; frame++)
    {
        MatrixRain_DrawFrame();
        _delay_ms(RAIN_DELAY);
    }

    max7219_clear();
}
