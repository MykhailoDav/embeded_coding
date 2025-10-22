#include <util/delay.h>
#include <stdint.h>
#include "TetrisButtons.h"
#include "TetrisEngine.h"
#include "MAX7219Matrix.h"
#include "MatrixRain.h"

#define BASE_DELAY 15

TetrisButtonManager btns;
TetrisGame game;

void setup()
{
    max7219_init(4, 3);
    max7219_clear();
    TetrisButtons_Init(&btns);

    MatrixRain_StartAnimation();

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