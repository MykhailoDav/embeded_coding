# I2C LCD - Weather Station with RTC

## Description
Comprehensive weather station displaying temperature, pressure, altitude, and real-time clock information on an I2C LCD display. Features BMP180 barometric sensor, DS1302 RTC, and interactive button control.

## Hardware Requirements
- ATmega328P microcontroller
- I2C LCD 16x2 display (PCF8574 backpack)
- BMP180 temperature and pressure sensor
- DS1302 Real-Time Clock module
- Push button for mode switching
- Breadboard and connecting wires

## Features
- **Sensor Readings:**
  - Temperature (°C)
  - Barometric pressure (Pa)
  - Calculated altitude (meters)
  - Real-time clock with date

- **Display Modes:**
  - **Temperature Mode:** Shows temp, pressure, altitude
  - **Time Mode:** Shows current time and date
  - Auto-switching between modes (configurable)

- **Button Controls:**
  - Single press: Manual mode switch
  - Double-click: Enable automatic mode
  - Long press: Additional functions

- **UART Commands:**
  - `time` - Show time only
  - `temp` - Show temperature only
  - `auto` - Enable auto-switching
  - `settime HH:MM:SS` - Set time
  - `setdate DD.MM.YYYY` - Set date
  - `help` - Show available commands

## Pin Configuration
### I2C Bus
| Device | SDA | SCL |
|--------|-----|-----|
| LCD    | A4  | A5  |
| BMP180 | A4  | A5  |

### DS1302 RTC
| Signal | ATmega328P Pin | Arduino Pin |
|--------|----------------|-------------|
| CLK    | PD5            | D5          |
| DAT    | PD6            | D6          |
| RST    | PD4            | D4          |

### Button
| Function | ATmega328P Pin | Arduino Pin |
|----------|----------------|-------------|
| Button   | PD2            | D2          |

## Libraries Included
- `i2c/` - I2C/TWI communication
- `lcd/` - LCD1602 driver
- `bmp180/` - BMP180 sensor library
- `DS1302/` - Real-Time Clock driver
- `Buttons/` - Button handling with debounce
- `USART/` - Serial communication

## Building and Uploading
```bash
cd i2c_lcd
pio run -t upload
```

## Simulation
SimulIDE simulation file: `i2c.sim1`

## Additional Documentation
- [Button Setup Guide](BUTTON_SETUP.md)
- [DS1302 Library Documentation](lib/DS1302/README.md)

## LCD Display Examples
### Temperature Mode:
```
🌡23.4C H:250m
P:101325 Pa
```

### Time Mode:
```
04.12.2025 Wed
    15:30:45
```

## Serial Commands Example
```
$: settime 15:30:00
$: setdate 04.12.2025
$: auto
```

## Learning Objectives
- I2C/TWI protocol implementation
- Multiple sensor integration
- Real-time clock usage
- LCD character display control
- Button state machine
- Command-line interface
- Data formatting and display
- Altitude calculation from pressure
