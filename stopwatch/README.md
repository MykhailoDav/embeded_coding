# Stopwatch - Digital Stopwatch with Display Effects

## Description
Fully-featured stopwatch with 4-digit seven-segment display, three control buttons, and LED garland effects. Includes configurable speed settings and visual feedback.

## Hardware Requirements
- ATmega328P microcontroller
- 4-digit seven-segment display
- 3 push buttons (Start, Stop, Reset)
- Display hardware (shift registers or direct control)
- Breadboard and connecting wires

## Pin Configuration
| Function | ATmega328P Pin | Arduino Pin |
|----------|----------------|-------------|
| Start    | PD2            | D2          |
| Stop     | PD3            | D3          |
| Reset    | PD4            | D4          |

## Features
- **Stopwatch Functions:**
  - Start/Resume timing
  - Stop/Pause timing
  - Reset to 0000

- **Advanced Features:**
  - Speed adjustment menu (50% - 200%)
  - LED garland animation effects
  - Button hold detection
  - Visual feedback modes

- **Display:**
  - 4-digit time display
  - Menu mode for settings
  - Animation effects

## Button Controls
- **Start:** Begin/resume counting
- **Stop:** Pause counting
- **Reset (Long Press):** Reset counter to zero
- **Menu Mode:** Adjust counting speed

## Libraries Used
- `Display.h` - Seven-segment display control
- `Buttons.h` - Button debouncing and state management

## Building and Uploading
```bash
cd stopwatch
pio run -t upload
```

## Simulation
SimulIDE simulation file: `sample.sim1`

## Learning Objectives
- State machine implementation
- Timer-based counting
- Multi-button input handling
- Display multiplexing
- Visual effects generation
- Menu system design
