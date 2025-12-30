# UART Test - Serial Command Interface

## Description
UART communication test project with command-line interface for controlling an LED. Demonstrates serial I/O, string parsing, and command processing.

## Hardware Requirements
- ATmega328P microcontroller
- LED with 220Ω resistor
- USB-to-Serial adapter (or Arduino USB)
- Breadboard and connecting wires

## Features
- **Serial Commands:**
  - `led on` - Turn LED on
  - `led off` - Turn LED off
  - `toggle` - Toggle LED state

- **UART Features:**
  - 9600 baud rate
  - Command echo
  - Status messages
  - Line-based input
  - Callback system

## Pin Configuration
| Function | ATmega328P Pin | Arduino Pin |
|----------|----------------|-------------|
| LED      | PB5            | D13         |
| TX       | PD1            | D1          |
| RX       | PD0            | D0          |

## Libraries Included
- `USART/` - Full-featured UART library
- `Logger/` - Debug logging utilities

## Building and Uploading
```bash
cd UART_test
pio run -t upload
```

## Usage
1. Upload the program
2. Open serial monitor at 9600 baud
3. Type commands:
   - `led on` → LED turns on
   - `led off` → LED turns off
   - `toggle` → LED toggles

## Learning Objectives
- UART/USART configuration
- Serial communication protocols
- String parsing and comparison
- Callback function implementation
- Printf redirection to serial
- Command-line interface design
