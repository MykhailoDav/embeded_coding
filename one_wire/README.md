# One Wire - Arduino Template Project

## Description
Template Arduino project using PlatformIO framework. Starting point for One-Wire protocol implementations such as DS18B20 temperature sensors.

## Hardware Requirements
- Arduino-compatible board
- One-Wire devices (e.g., DS18B20 temperature sensor)
- 4.7kΩ pull-up resistor
- Breadboard and connecting wires

## Features
- Arduino framework compatibility
- PlatformIO build system
- C++ source structure
- Ready for One-Wire library integration

## Typical One-Wire Applications
- DS18B20 temperature sensors
- DS2431 EEPROM
- iButton devices
- Multi-sensor networks

## One-Wire Connection
```
Arduino Pin → 4.7kΩ → +5V
           ↓
      One-Wire Device
           ↓
          GND
```

## Building and Uploading
```bash
cd one_wire
pio run -t upload
```

## Getting Started
1. Install One-Wire library (e.g., OneWire, DallasTemperature)
2. Connect One-Wire device with pull-up resistor
3. Implement sensor reading code in `main.cpp`
4. Build and upload

## Recommended Libraries
- **OneWire** - Basic One-Wire protocol
- **DallasTemperature** - DS18B20 sensor library

## Example Use Case
```cpp
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  delay(1000);
}
```

## Learning Objectives
- One-Wire protocol understanding
- Arduino framework usage
- Digital temperature sensing
- Pull-up resistor requirements
- Multi-device bus addressing

## Notes
This is a template project. Implement specific One-Wire functionality based on your requirements.
