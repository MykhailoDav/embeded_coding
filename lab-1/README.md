# Lab 1 - RGB LED Control

## Description
Basic AVR project demonstrating RGB LED control using ATmega328P microcontroller. The program cycles through red, green, and blue colors with a 500ms delay between each color.

## Hardware Requirements
- ATmega328P microcontroller
- RGB LED (common cathode)
- 3x 220Ω resistors (current limiting for each LED color)
- Breadboard and connecting wires

## Pin Configuration
| LED Color | ATmega328P Pin | Arduino Pin |
|-----------|----------------|-------------|
| Red       | PC3            | A3          |
| Green     | PC2            | A2          |
| Blue      | PC1            | A1          |

## Circuit Connection
```
RGB LED:
  Red   → 220Ω → PC3
  Green → 220Ω → PC2
  Blue  → 220Ω → PC1
  GND   → GND
```

## Features
- Sequential color cycling (Red → Green → Blue)
- 500ms delay between color changes
- Direct port manipulation for efficient control

## Building and Uploading
This project uses PlatformIO. To build and upload:

```bash
cd lab-1
pio run -t upload
```

## Simulation
SimulIDE simulation file included: `lab_1.sim1`

## Learning Objectives
- Understanding GPIO configuration on AVR
- Port manipulation using DDR and PORT registers
- Basic timing using delay functions
- RGB LED control fundamentals
