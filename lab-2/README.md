# Lab 2 - Seven Segment Display

## Description
AVR project implementing a seven-segment display controller. Demonstrates direct segment control and digit pattern generation using ATmega328P.

## Hardware Requirements
- ATmega328P microcontroller
- Common cathode seven-segment display
- 7x 220Ω resistors (current limiting for segments)
- Breadboard and connecting wires

## Pin Configuration
| Segment | ATmega328P Pin | Arduino Pin |
|---------|----------------|-------------|
| A       | PD0            | D0          |
| B       | PD1            | D1          |
| C       | PD2            | D2          |
| D       | PD3            | D3          |
| E       | PD4            | D4          |
| F       | PD5            | D5          |
| G       | PD6            | D6          |

## Features
- Digit patterns for 0-9
- Segment control functions
- Clear display functionality
- Common cathode configuration

## Seven-Segment Layout
```
    A
   ---
F |   | B
   -G-
E |   | C
   ---
    D
```

## Building and Uploading
```bash
cd lab-2
pio run -t upload
```

## Simulation
SimulIDE simulation files included:
- `lab_2.sim1` - Main simulation
- `blinker.sim1` - Additional example

## Learning Objectives
- Seven-segment display operation
- Bit manipulation and masking
- Look-up tables for pattern storage
- Port register operations
