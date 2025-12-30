# 4-Bit LED Digital Module

## Description
Control a 4-digit seven-segment display module using 74HC595 shift registers. Demonstrates serial data transmission and multiplexed display control.

## Hardware Requirements
- ATmega328P microcontroller
- 4-digit seven-segment display with 74HC595 shift registers
- Breadboard and connecting wires

## Pin Configuration
| Signal | ATmega328P Pin | Arduino Pin | Description |
|--------|----------------|-------------|-------------|
| SCLK   | PD2            | D2          | Serial Clock |
| RCLK   | PD3            | D3          | Register Clock (Latch) |
| DIO    | PD4            | D4          | Data Input |

## Features
- 74HC595 shift register control
- Serial data transmission
- 7-segment digit patterns (0-9)
- Common cathode display support
- Efficient bit-shifting operations

## 74HC595 Control Protocol
1. Set data bit on DIO
2. Pulse SCLK (shift data into register)
3. Pulse RCLK (latch data to output)

## Building and Uploading
```bash
cd 4-bit-led-digital-module
pio run -t upload
```

## Learning Objectives
- Shift register operation
- Serial communication protocols
- Display multiplexing
- Reducing pin count with shift registers
- Timing and synchronization
