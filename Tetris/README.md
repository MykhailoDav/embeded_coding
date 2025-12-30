# Tetris - Classic Game Implementation

## Description
Full implementation of the classic Tetris game on an 8x32 LED matrix display using MAX7219 drivers. Features complete game mechanics, button controls, and Matrix Rain intro animation.

## Hardware Requirements
- ATmega328P microcontroller
- MAX7219-based 8x32 LED matrix (4 modules of 8x8)
- 4-5 push buttons (game controls)
- 5V power supply
- Breadboard and connecting wires

## Features
- **Complete Tetris Gameplay:**
  - Seven tetromino shapes (I, O, T, S, Z, J, L)
  - Piece rotation
  - Line clearing
  - Score tracking
  - Increasing difficulty

- **Visual Effects:**
  - Matrix Rain intro animation
  - Smooth piece movement
  - Clear line animations

- **Controls:**
  - Move left/right
  - Rotate piece
  - Soft drop
  - Hard drop (optional)

## Libraries Included
- `TetrisEngine/` - Core game logic
- `TetrisButtons/` - Button input handling
- `MAX7219Matrix/` - Display driver
- `MatrixRain/` - Intro animation effect
- `SimpleSPI/` - SPI communication

## Pin Configuration
### Display (MAX7219)
| Signal | ATmega328P | Arduino |
|--------|------------|---------|
| DIN    | MOSI       | D11     |
| CLK    | SCK        | D13     |
| CS     | Custom     | Custom  |

### Buttons
Refer to `TetrisButtons.h` for button pin assignments.

## Building and Uploading
```bash
cd Tetris
pio run -t upload
```

## Game Controls
- **Left Button:** Move piece left
- **Right Button:** Move piece right
- **Rotate Button:** Rotate piece clockwise
- **Down Button:** Soft drop (faster fall)
- **Drop Button:** Hard drop (instant)

## Learning Objectives
- Game state management
- Real-time embedded game development
- Matrix display control
- Button input handling
- Collision detection
- Animation implementation
- Modular code architecture

## Credits
Classic Tetris game adapted for AVR microcontroller platform.
