#include "TetrisButtons.h"
#include <avr/io.h>

void TetrisButtons_Init(TetrisButtonManager *mgr)
{
    DDRD &= ~((1 << PIN_LEFT) | (1 << PIN_RIGHT) | (1 << PIN_DOWN) | (1 << PIN_ROTATE));
    PORTD |= ((1 << PIN_LEFT) | (1 << PIN_RIGHT) | (1 << PIN_DOWN) | (1 << PIN_ROTATE));

    // === Structure binding ===
    mgr->defs[BTN_LEFT] = (ButtonDef){.state = &mgr->states[BTN_LEFT], .pin = PIN_LEFT};
    mgr->defs[BTN_RIGHT] = (ButtonDef){.state = &mgr->states[BTN_RIGHT], .pin = PIN_RIGHT};
    mgr->defs[BTN_DOWN] = (ButtonDef){.state = &mgr->states[BTN_DOWN], .pin = PIN_DOWN};
    mgr->defs[BTN_ROTATE] = (ButtonDef){.state = &mgr->states[BTN_ROTATE], .pin = PIN_ROTATE};

    Buttons_Init(mgr->defs, BTN_COUNT);
}

void TetrisButtons_Update(TetrisButtonManager *mgr, uint32_t millis)
{
    Buttons_Update(millis);
}

bool TetrisButtons_IsPressed(TetrisButtonManager *mgr, TetrisButton btn)
{
    return mgr->states[btn].released;
}

bool TetrisButtons_IsHold(TetrisButtonManager *mgr, TetrisButton btn)
{
    return mgr->states[btn].hold;
}

bool TetrisButtons_IsLongPress(TetrisButtonManager *mgr, TetrisButton btn)
{
    return mgr->states[btn].long_press;
}

bool TetrisButtons_IsReleased(TetrisButtonManager *mgr, TetrisButton btn)
{
    return mgr->states[btn].released;
}
