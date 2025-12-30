# Counter - Button-Controlled Counter

## Description
Single-digit counter (0-9) with increment and reset buttons. Features debouncing, auto-repeat functionality, and long-press reset capability.

## Hardware Requirements
- ATmega328P microcontroller
- Common cathode seven-segment display
- 2 push buttons (Increment and Reset)
- 7x 220Ω resistors (for segments)
- Breadboard and connecting wires

## Pin Configuration
### Buttons
| Function  | ATmega328P Pin | Arduino Pin |
|-----------|----------------|-------------|
| Increment | PB0            | D8          |
| Reset     | PB1            | D9          |

### Seven-Segment Display
Connected to PORTD (PD0-PD6)

## Features
- **Increment Button:**
  - Short press: Increment counter by 1
  - Hold: Auto-increment with 1 second delay
  
- **Reset Button:**
  - Long press (>3 seconds): Reset counter to 0

- Counter wraps from 9 to 0
- Internal pull-up resistors enabled
- Software debouncing
- Millisecond timer for precise timing

## Building and Uploading
```bash
cd counter
pio run -t upload
```

## Simulation
SimulIDE simulation file: `increment.sim1`

## Learning Objectives
- Button debouncing techniques
- Timer-based auto-repeat
- State management
- Pull-up resistor usage
- Long-press detection
