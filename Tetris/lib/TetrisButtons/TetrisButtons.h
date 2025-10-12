#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "Buttons.h"

#define PIN_LEFT PD2 // D2
#define PIN_RIGHT PD3 // D3
#define PIN_DOWN PD4 // D4
#define PIN_ROTATE PD5 // D5

// === Кнопки тетрісу ===
// 4 фізичні кнопки: LEFT, RIGHT, DOWN, ROTATE/PAUSE
typedef enum
{
    BTN_LEFT = 0,
    BTN_RIGHT,
    BTN_DOWN,
    BTN_ROTATE, // коротке = rotate, довге = pause
    BTN_COUNT
} TetrisButton;

// === Менеджер кнопок ===
typedef struct
{
    ButtonState states[BTN_COUNT];
    ButtonDef defs[BTN_COUNT];
} TetrisButtonManager;

// === API ===
void TetrisButtons_Init(TetrisButtonManager *mgr);
void TetrisButtons_Update(TetrisButtonManager *mgr, uint32_t millis);

// === Перевірки ===
bool TetrisButtons_IsPressed(TetrisButtonManager *mgr, TetrisButton btn);
bool TetrisButtons_IsHold(TetrisButtonManager *mgr, TetrisButton btn);
bool TetrisButtons_IsLongPress(TetrisButtonManager *mgr, TetrisButton btn);
bool TetrisButtons_IsReleased(TetrisButtonManager *mgr, TetrisButton btn);

