#include "IRSensor.h"
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    IR_Init();

    for (;;)
    {
        // нічого не робимо — просто чекаємо переривань

    }
}
