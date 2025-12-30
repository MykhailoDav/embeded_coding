# LED Display 8x32 - Matrix Display Controller

## Description
Driver and display controller for 8x32 LED matrix using MAX7219 drivers. Supports text scrolling, graphics, and custom fonts.

## Hardware Requirements
- ATmega328P microcontroller
- MAX7219-based 8x32 LED matrix modules
- SPI connection
- 5V power supply
- Breadboard and connecting wires

## Features
- **Display Control:**
  - 8x32 pixel resolution
  - MAX7219 driver support
  - Brightness control
  - Display matrix manipulation

- **Graphics:**
  - Custom 8x8 font rendering
  - Scrolling text
  - Pattern display
  - Animation support

- **Communication:**
  - SPI protocol
  - Simple SPI library included

## Libraries Included
- `Font8x8/` - 8x8 pixel font definitions
- `MAX7219Matrix/` - MAX7219 driver library
- Custom SPI implementation

## Pin Configuration
Typical SPI connection to MAX7219:
| Signal | ATmega328P | Arduino |
|--------|------------|---------|
| DIN    | MOSI       | D11     |
| CLK    | SCK        | D13     |
| CS     | SS         | D10     |

## Building and Uploading
```bash
cd led_display_8_32
pio run -t upload
```

## Learning Objectives
- MAX7219 LED driver control
- SPI communication
- Matrix display multiplexing
- Font rendering
- Animation techniques
- Modular library design
