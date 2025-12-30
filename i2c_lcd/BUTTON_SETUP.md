# Weather Station with Button Control

## 📌 Button Connection

The button is connected to pin **PD2 (D2)** on Arduino/ATmega328P.

### Connection Diagram:

```
Button:
┌─────────────┐
│   BUTTON    │
└──┬─────────┬┘
   │         │
   │         └──────── GND
   │
   └────────────────── PD2 (D2)

Internal pull-up resistor enabled programmatically!
```

### Connection Details:

| Component | Arduino Pin | ATmega328P Pin |
|-----------|-------------|----------------|
| Button (one contact) | D2 | PD2 |
| Button (second contact) | GND | GND |

**Important:** No external resistor needed - internal pull-up is used!

---

## 🎮 Button Functions

### 1️⃣ Single Press
- Switches display mode manually
- Disables automatic mode
- Shows mode name for 1 second

**Modes:**
- 🌡️ **TEMPERATURE** - temperature, pressure, altitude
- 🕐 **TIME** - time and date

### 2️⃣ Double Press (within 400 ms)
- Enables automatic mode
- Modes switch every 2 seconds
- Shows "AUTOMATIC MODE" for 1 second

---

## ⏱️ Display Updates

- **Temperature:** updates every **500 ms**
- **Time:** updates **continuously** (every millisecond via Timer0)
- **Timer0:** configured for interrupt every **1 ms** for accurate time counting

---

## 📟 Usage Example

```c
// Timer setup for millis()
TCCR0A = (1 << WGM01);  // CTC mode
TCCR0B = (1 << CS01) | (1 << CS00);  // Prescaler 64
OCR0A = 249;  // 250 ticks = 1 ms
TIMSK0 = (1 << OCIE0A);  // Interrupt

// Button initialization
DDRD &= ~(1 << BUTTON_PIN);  // Input
PORTD |= (1 << BUTTON_PIN);  // Pull-up
Buttons_Init(&button, 1);

// Button processing in loop
Buttons_Update(millis());

if (button_state.double_click) {
    // Automatic mode
}
if (button_state.released) {
    // Switch mode
}
```

---

## 🔧 Buttons Library Configuration

### Added Features:
- ✅ Double-click detection
- ✅ Double-click timeout: **400 ms**
- ✅ Click counter
- ✅ Last release time

### New fields in `ButtonState`:
```c
bool double_click;              // Double-click detected
uint32_t last_release_time;     // Last release time
uint8_t click_count;            // Click counter
```

---

## 🖥️ LCD Display

### TEMPERATURE Mode:
```
🌡23.4C H:250m
P:101325 Pa
```

### TIME Mode:
```
04.12.2025 Wed
    15:30:45
```

### When switching (1 sec):
```
  TEMPERATURE
     MODE
```
or
```
   AUTOMATIC
     MODE
```

---

## 🚀 Getting Started

1. Connect button to PD2 (D2) and GND
2. Upload the code
3. Press button twice - automatic mode will activate
4. Press once - switch mode manually

**Done!** ✨
