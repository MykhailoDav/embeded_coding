# UART Display - Serial-Controlled LED Matrix

## Description
8x32 LED matrix display controlled via UART serial commands. Features scrolling text, command interpreter, and configurable display settings.

## Hardware Requirements
- ATmega328P microcontroller
- MAX7219-based 8x32 LED matrix
- USB-to-Serial adapter
- 5V power supply
- Breadboard and connecting wires

## Features
- **Display Control:**
  - Scrolling text messages
  - Adjustable scroll speed
  - Text wrapping
  - Display on/off control
  - Brightness adjustment

- **Serial Commands:**
  - Set display text
  - Configure scroll speed
  - Start/stop scrolling
  - Clear display
  - Help system

- **Text Rendering:**
  - 8x8 custom font
  - Variable character spacing
  - Automatic text width calculation
  - Smooth scrolling

## Libraries Included
- `Font8x8/` - 8x8 pixel font
- `MAX7219Matrix/` - Display driver
- `SimpleSPI/` - SPI communication
- `USART/` - Serial communication

## Pin Configuration
### Display (MAX7219/SPI)
| Signal | ATmega328P | Arduino |
|--------|------------|---------|
| DIN    | MOSI       | D11     |
| CLK    | SCK        | D13     |
| CS     | Custom     | Custom  |

### Serial
| Signal | ATmega328P | Arduino |
|--------|------------|---------|
| TX     | PD1        | D1      |
| RX     | PD0        | D0      |

## Building and Uploading
```bash
cd UART_display
pio run -t upload
```

## Serial Commands
Connect at 9600 baud and use commands like:
- `text:YOUR MESSAGE` - Set display text
- `speed:100` - Set scroll speed (ms)
- `scroll:on` - Enable scrolling
- `scroll:off` - Disable scrolling
- `clear` - Clear display
- `help` - Show available commands

## Usage Example
```
$: text:HELLO WORLD
$: speed:50
$: scroll:on
```

## Learning Objectives
- UART command parsing
- Real-time text scrolling
- SPI display control
- String processing
- Font rendering
- Timer-based animations
- Command-line interface design
