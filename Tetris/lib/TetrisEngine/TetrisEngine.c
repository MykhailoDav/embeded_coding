#include "TetrisEngine.h"
#include <string.h>
#include <stdlib.h>

static const uint8_t SHAPES[7][4][4] = {
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, // L
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, // J
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // I
};

// === Перевірка розміщення ===
static bool can_place(const TetrisGame *g, int x, int y)
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
        {
            if (!shape_cell(g->current.shape, g->current.rotation, r, c))
                continue;

            int fx = x + c;
            int fy = y + r;

            if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT)
                return false;
            if (fy >= 0 && g->field[fy][fx] == CELL_FILLED)
                return false;
        }
    return true;
}

// === Додавання фігури в поле ===
static void merge_piece(TetrisGame *g)
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
        {
            if (!shape_cell(g->current.shape, g->current.rotation, r, c))
                continue;

            int fx = g->current.x + c;
            int fy = g->current.y + r;
            if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH)
                g->field[fy][fx] = CELL_FILLED;
        }
}

// === Створення нової фігури ===
// === Створення нової фігури ===
static void spawn_piece(TetrisGame *g)
{
    // поточна фігура стає тією, що була "наступною"
    g->current.shape = g->nextShape;
    g->current.x = FIELD_WIDTH / 2 - 2;
    g->current.y = -1;
    g->current.rotation = 0;

    // згенерувати нову наступну
    g->nextShape = rand() % 7;
}

void Tetris_Init(TetrisGame *g)
{
    memset(g->field, 0, sizeof(g->field));
    g->paused = false;
    g->gameOver = false;
    g->nextShape = rand() % 7;
    spawn_piece(g);
}

void Tetris_Update(TetrisGame *g, TetrisButtonManager *btns)
{
    static bool downWasHeld = false; // чи був soft-drop перед відпусканням
    static uint32_t tick = 0, lastFall = 0;

    if (g->gameOver)
    {
        // Якщо натиснути кнопку ROTATE — перезапуск гри
        if (TetrisButtons_IsPressed(btns, BTN_ROTATE))
        {
            Tetris_Init(g); // очистити поле і створити нову фігуру
        }
        return;
    }

    // --- Пауза ---
    if (TetrisButtons_IsLongPress(btns, BTN_ROTATE))
        g->paused = !g->paused;
    if (g->paused)
        return;

    // --- Рух ---
    if (TetrisButtons_IsPressed(btns, BTN_LEFT) && can_place(g, g->current.x - 1, g->current.y))
        g->current.x--;
    if (TetrisButtons_IsPressed(btns, BTN_RIGHT) && can_place(g, g->current.x + 1, g->current.y))
        g->current.x++;

    // --- Обертання ---
    if (TetrisButtons_IsPressed(btns, BTN_ROTATE))
    {
        uint8_t oldRot = g->current.rotation;
        g->current.rotation = (g->current.rotation + 1) & 3;

        // простий відкат, якщо не влазить
        if (!can_place(g, g->current.x, g->current.y))
            g->current.rotation = oldRot;
    }

    tick++;

    // --- Soft drop (утримання кнопки) ---
    uint16_t fallDelay = FALL_DELAY;
    if (TetrisButtons_IsHold(btns, BTN_DOWN))
    {
        fallDelay = FALL_DELAY / 1000; // пришвидшення
        downWasHeld = true;            // запам’ятати, що це був soft-drop
    }

    // --- Рух вниз за таймером ---
    if (tick - lastFall > fallDelay)
    {
        lastFall = tick;
        if (can_place(g, g->current.x, g->current.y + 1))
        {
            g->current.y++;
        }
        else
        {
            merge_piece(g);
            clear_full_lines(g);
            spawn_piece(g);
            if (!can_place(g, g->current.x, g->current.y))
                g->gameOver = true;
        }
    }

    // --- Hard drop ---
    if (TetrisButtons_IsPressed(btns, BTN_DOWN))
    {
        // Виконується лише якщо не було утримання
        if (!downWasHeld)
        {
            while (can_place(g, g->current.x, g->current.y + 1))
                g->current.y++;

            merge_piece(g);
            clear_full_lines(g);
            spawn_piece(g);
            if (!can_place(g, g->current.x, g->current.y))
                g->gameOver = true;
        }
    }

    // --- Скидання після відпускання ---
    if (TetrisButtons_IsReleased(btns, BTN_DOWN))
    {
        downWasHeld = false;
    }
}

// === Малювання на 8×32 ===
void Tetris_Draw(const TetrisGame *g)
{

    if (g->gameOver)
    {
        draw_LOSE();
        return;
    }

    matrix_clear_buffer();

    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (SHAPES[g->nextShape][r][c])
                matrix_set_pixel(c + 5, r, 1);

    // Поле
    for (int y = 0; y < FIELD_HEIGHT; y++)
        for (int x = 0; x < FIELD_WIDTH; x++)
            if (g->field[y][x] == CELL_FILLED)
                matrix_set_pixel(x, y, 1);

    // Поточна фігура
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
        {
            if (!shape_cell(g->current.shape, g->current.rotation, r, c))
                continue;

            int fx = g->current.x + c;
            int fy = g->current.y + r;
            if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH)
                matrix_set_pixel(fx, fy, 1);
        }

    matrix_draw();
}

// ==== ХЕЛПЕР РОТАЦІЇ 4x4 ====
// Повертає 0/1 для клітинки з урахуванням rotation (0..3)
static inline uint8_t shape_cell(uint8_t shape, uint8_t rotation, int r, int c)
{
    // r,c у діапазоні 0..3
    switch (rotation & 3)
    {
    case 0: // без повороту
        return SHAPES[shape][r][c];
    case 1: // +90°
        // (r, c) у повернутій -> оригінал: (3 - c, r)
        return SHAPES[shape][3 - c][r];
    case 2: // +180°
        // (r, c) -> (3 - r, 3 - c)
        return SHAPES[shape][3 - r][3 - c];
    case 3: // +270°
        // (r, c) -> (c, 3 - r)
        return SHAPES[shape][c][3 - r];
    }
    return 0;
}

// === Знищення заповнених ліній ===
static void clear_full_lines(TetrisGame *g)
{
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--)
    {
        bool full = true;
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            if (g->field[y][x] == CELL_EMPTY)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            // зрушити всі рядки вище вниз
            for (int yy = y; yy > 0; yy--)
                memcpy(g->field[yy], g->field[yy - 1], FIELD_WIDTH * sizeof(CellType));

            // очистити верхній рядок
            memset(g->field[0], CELL_EMPTY, FIELD_WIDTH * sizeof(CellType));

            // після зсуву перевірити той самий рядок ще раз
            y++;
        }
    }
}

// === Відображення слова LOSE ===
static void draw_LOSE(void)
{
    matrix_clear_buffer();

    // Кожна буква 5x7 (у нас поле 8x32 — влізе ідеально)
    const uint8_t L[7] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00};
    const uint8_t O[7] = {0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00};
    const uint8_t S[7] = {0x3C, 0x40, 0x3C, 0x02, 0x42, 0x3C, 0x00};
    const uint8_t E[7] = {0x7E, 0x40, 0x7C, 0x40, 0x7E, 0x00, 0x00};

    const uint8_t *letters[4] = {L, O, S, E};

    // Відстань між буквами = 1 рядок
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t yOffset = i * 8; // 7 + 1
        for (uint8_t row = 0; row < 7; row++)
        {
            uint8_t bits = letters[i][row];
            for (uint8_t col = 0; col < 8; col++)
            {
                if (bits & (1 << (7 - col)))
                    matrix_set_pixel(col, yOffset + row, 1);
            }
        }
    }

    matrix_draw();
}
