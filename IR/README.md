# IR - Infrared Sensor Interface

## Description
Infrared sensor interface using interrupt-driven detection. Demonstrates IR signal reception and UART communication for debugging.

## Hardware Requirements
- ATmega328P microcontroller
- IR receiver module (e.g., TSOP4838, VS1838B)
- UART connection (for debugging)
- Breadboard and connecting wires

## Features
- **IR Reception:**
  - Interrupt-driven IR signal detection
  - Signal decoding capability
  - Low-power operation

- **Communication:**
  - UART serial output
  - Debug messaging
  - Status reporting

## Libraries Included
- `IRSensor/` - IR sensor driver with interrupt handling
- `UART/` - Serial communication library

## Pin Configuration
Refer to `IRSensor.h` for specific pin assignments.

## Building and Uploading
```bash
cd IR
pio run -t upload
```

## Serial Monitor
Connect to serial monitor at the baud rate specified in the code to view IR signal data.

## Learning Objectives
- External interrupt handling
- IR protocol decoding
- Interrupt service routines (ISR)
- UART communication
- Signal timing analysis
