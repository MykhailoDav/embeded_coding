# Embedded Systems Programming - AVR Projects Collection

A comprehensive collection of AVR microcontroller projects demonstrating embedded systems programming concepts, from basic GPIO control to complex game implementations and sensor integrations.

## 🎯 Overview

This workspace contains 15 embedded systems projects built for the ATmega328P microcontroller using PlatformIO. Projects range from beginner-friendly LED control to advanced applications including games, sensor systems, and communication protocols.

## 📚 Projects

### Beginner Level

#### [lab-1](lab-1/) - RGB LED Control
Basic RGB LED color cycling demonstrating GPIO configuration and port manipulation.
- **Concepts:** GPIO, port registers, delays
- **Hardware:** RGB LED, ATmega328P

#### [lab-2](lab-2/) - Seven Segment Display
Single digit seven-segment display control with digit pattern generation.
- **Concepts:** Bit manipulation, lookup tables, display control
- **Hardware:** 7-segment display

#### [counter](counter/) - Button Counter
Single-digit counter with increment/reset buttons, debouncing, and auto-repeat.
- **Concepts:** Button debouncing, state management, timers
- **Hardware:** 7-segment display, 2 buttons

### Intermediate Level

#### [4-bit-led-digital-module](4-bit-led-digital-module/) - Shift Register Display
4-digit seven-segment display using 74HC595 shift registers.
- **Concepts:** Shift registers, serial data transmission
- **Hardware:** 4-digit display, 74HC595

#### [stopwatch](stopwatch/) - Digital Stopwatch
Full-featured stopwatch with 4-digit display, three buttons, and visual effects.
- **Concepts:** State machines, timing, display multiplexing
- **Hardware:** 4-digit display, 3 buttons

#### [IR](IR/) - Infrared Sensor
Interrupt-driven IR sensor interface with UART debugging.
- **Concepts:** External interrupts, IR protocols, UART
- **Hardware:** IR receiver, UART adapter

#### [UART_test](UART_test/) - Serial Command Interface
LED control via serial commands demonstrating UART communication.
- **Concepts:** UART/USART, string parsing, callbacks
- **Hardware:** LED, USB-to-Serial adapter

### Advanced Level

#### [led_display_8_32](led_display_8_32/) - LED Matrix Controller
8x32 LED matrix display with scrolling text and graphics using MAX7219 drivers.
- **Concepts:** SPI, matrix multiplexing, font rendering
- **Hardware:** 8x32 LED matrix, MAX7219 modules

#### [Tetris](Tetris/) - Game Implementation
Complete Tetris game with full mechanics, animations, and button controls.
- **Concepts:** Game logic, real-time systems, collision detection
- **Hardware:** 8x32 LED matrix, 4-5 buttons

#### [UART_display](UART_display/) - Serial LED Matrix
UART-controlled 8x32 LED matrix with scrolling text and command interface.
- **Concepts:** Command parsing, text rendering, SPI + UART
- **Hardware:** 8x32 LED matrix, USB-to-Serial

### Sensor Integration

#### [i2c_lcd](i2c_lcd/) - Weather Station
Weather station with BMP180 sensor, DS1302 RTC, and LCD display.
- **Concepts:** I2C/TWI, multiple sensors, RTC, altitude calculation
- **Hardware:** I2C LCD, BMP180, DS1302, button

#### [I2s_display_hardware](I2s_display_hardware/) - I2C LCD Driver
Low-level I2C LCD driver implementation using PCF8574 I/O expander.
- **Concepts:** TWI protocol, PCF8574, LCD 4-bit mode
- **Hardware:** I2C LCD 16x2

### Template Projects

#### [Lab-4_1](Lab-4_1/) & [lab-4_2](lab-4_2/)
Laboratory assignment projects for intermediate AVR programming.

#### [one_wire](one_wire/)
Arduino template for One-Wire protocol devices (DS18B20, etc.).
- **Concepts:** One-Wire protocol, Arduino framework
- **Hardware:** DS18B20 or other One-Wire devices

## 🛠️ Development Environment

### Requirements
- **PlatformIO** - Build system and project manager
- **AVR-GCC** - Compiler toolchain
- **AVRDUDE** - Upload utility
- **SimulIDE** (optional) - Circuit simulator

### Installation

1. **Install PlatformIO:**
   ```bash
   # Using pip
   pip install platformio
   
   # Or install PlatformIO IDE (VS Code extension)
   ```

2. **Clone or navigate to workspace:**
   ```bash
   cd /path/to/embeded_coding
   ```

3. **Build a project:**
   ```bash
   cd project-name
   pio run
   ```

4. **Upload to device:**
   ```bash
   pio run -t upload
   ```

## 📋 Common Hardware

Most projects use:
- **Microcontroller:** ATmega328P (Arduino Uno compatible)
- **Programmer:** Arduino bootloader or USBasp
- **Power:** 5V USB or external supply
- **Common components:** LEDs, resistors, buttons, displays

## 🔌 Standard Pin Mappings

### ATmega328P to Arduino Pin Reference
| ATmega Pin | Arduino | Common Use |
|------------|---------|------------|
| PB0-PB5    | D8-D13  | Digital I/O, SPI |
| PC0-PC5    | A0-A5   | Analog/Digital, I2C |
| PD0-PD7    | D0-D7   | Digital I/O, UART |

### Common Protocols
- **SPI:** MOSI (D11), MISO (D12), SCK (D13), SS (D10)
- **I2C:** SDA (A4), SCL (A5)
- **UART:** TX (D1), RX (D0)

## 📖 Learning Path

### Recommended Order:
1. **Start here:** `lab-1` → `lab-2` → `counter`
2. **Displays:** `4-bit-led-digital-module` → `stopwatch`
3. **Communication:** `UART_test` → `IR` → `UART_display`
4. **Advanced displays:** `led_display_8_32` → `Tetris`
5. **Sensors:** `I2s_display_hardware` → `i2c_lcd`
6. **Additional:** `Lab-4_1`, `lab-4_2`, `one_wire`

## 🎓 Key Concepts Covered

- **GPIO Control:** Direct port manipulation, pin configuration
- **Timers:** Delays, millisecond counters, PWM
- **Interrupts:** External interrupts, timer interrupts
- **Displays:** 7-segment, LED matrix, LCD
- **Communication:** UART, SPI, I2C/TWI
- **Sensors:** Temperature, pressure, IR
- **Input:** Buttons with debouncing, state machines
- **Advanced:** Game logic, animations, command parsing

## 🔧 Build System

All projects use PlatformIO with `platformio.ini` configuration:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino  ; or native AVR
```

## 📁 Project Structure

Each project follows this structure:
```
project-name/
├── README.md           # Project documentation
├── platformio.ini      # PlatformIO configuration
├── src/
│   └── main.c          # Main source file
├── lib/                # Custom libraries
├── include/            # Header files
└── *.sim1              # SimulIDE simulation (optional)
```

## 🐛 Troubleshooting

### Upload Issues
- Check USB connection
- Verify board selection in `platformio.ini`
- Try different USB cable/port
- Check bootloader presence

### Compilation Errors
- Ensure AVR toolchain is installed
- Check include paths
- Verify library dependencies

### Hardware Issues
- Check wiring against pin configurations
- Verify voltage levels (5V for ATmega328P)
- Test with minimal circuit first

## 📚 Resources

- [ATmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [AVR Libc Reference](https://www.nongnu.org/avr-libc/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

## 🤝 Contributing

When adding new projects:
1. Include comprehensive README.md
2. Add SimulIDE simulation if applicable
3. Follow existing project structure
4. Comment code in English
5. Test thoroughly before committing

## 📄 License

Educational projects for embedded systems learning.

## ✨ Features Highlight

- ✅ All documentation in English
- ✅ Clean project structure
- ✅ Comprehensive README files for each project
- ✅ SimulIDE simulations included
- ✅ PlatformIO build system
- ✅ Modular library design
- ✅ Real hardware tested

---

**Last Updated:** December 31, 2025
**Microcontroller:** ATmega328P
**Build System:** PlatformIO
**Language:** C/C++
