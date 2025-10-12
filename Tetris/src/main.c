#include <util/delay.h>
#include <stdint.h>
#include "TetrisButtons.h"
#include "TetrisEngine.h"
#include "MAX7219Matrix.h"

#define BASE_DELAY 15

TetrisButtonManager btns;
TetrisGame game;

void setup()
{
    max7219_init(4, 0);
    max7219_clear();
    TetrisButtons_Init(&btns);

    for (uint8_t y = 0; y < 32; y++)
    {
        matrix_clear_buffer();
        matrix_set_pixel(3, y, 1);
        matrix_draw();
        _delay_ms(10);
    }

    Tetris_Init(&game);
}

void loop()
{
    static uint32_t ms = 0;
    _delay_ms(BASE_DELAY);
    ms += BASE_DELAY;

    TetrisButtons_Update(&btns, ms);
    Tetris_Update(&game, &btns);
    Tetris_Draw(&game);
}

int main(void)
{
    setup();
    for (;;)
    {
        loop();
    }

    return 0;
}