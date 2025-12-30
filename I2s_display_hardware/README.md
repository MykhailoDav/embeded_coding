# I2C Display Hardware - Basic LCD Interface

## Description
Minimal implementation of I2C LCD1602 display driver using PCF8574 I/O expander. Demonstrates low-level I2C communication and LCD control.

## Hardware Requirements
- ATmega328P microcontroller
- LCD 16x2 with I2C backpack (PCF8574)
- Breadboard and connecting wires

## Features
- **I2C Communication:**
  - TWI (Two-Wire Interface) initialization
  - Low-level I2C read/write operations
  - PCF8574 I/O expander control

- **LCD Control:**
  - 4-bit mode operation
  - Character display
  - Cursor positioning
  - Backlight control

## Pin Configuration
| Signal | ATmega328P Pin | Arduino Pin |
|--------|----------------|-------------|
| SDA    | PC4            | A4          |
| SCL    | PC5            | A5          |

## I2C Address
Default PCF8574 address: `0x27` (may vary, check your module)

## Building and Uploading
```bash
cd I2s_display_hardware
pio run -t upload
```

## Display Output
The program displays:
```
Hello, World!
I2C LCD1602
```

## Learning Objectives
- I2C/TWI protocol implementation
- PCF8574 I/O expander usage
- LCD 4-bit mode operation
- Low-level peripheral control
- Bit manipulation for LCD control signals
- Hardware initialization sequences

## Notes
This is a minimal "bare-metal" implementation useful for understanding I2C and LCD fundamentals. For production use, consider using higher-level libraries.
