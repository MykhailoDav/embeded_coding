#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "TetrisButtons.h"
#include "MAX7219Matrix.h"

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 32
#define FALL_DELAY 20
typedef enum
{
    CELL_EMPTY = 0,
    CELL_FILLED
} CellType;

typedef struct
{
    int8_t x, y;
    uint8_t shape;    // 0–6
    uint8_t rotation; // 0–3
} TetrisPiece;

typedef struct
{
    CellType field[FIELD_HEIGHT][FIELD_WIDTH];
    TetrisPiece current;
    bool paused;
    bool gameOver;
    uint8_t nextShape;
} TetrisGame;

void Tetris_Init(TetrisGame *game);
void Tetris_Update(TetrisGame *game, TetrisButtonManager *buttons);
void Tetris_Draw(const TetrisGame *game);

static inline uint8_t shape_cell(uint8_t shape, uint8_t rotation, int r, int c);

static void clear_full_lines(TetrisGame *g);
static void draw_LOSE(void);
static void draw_P(void);