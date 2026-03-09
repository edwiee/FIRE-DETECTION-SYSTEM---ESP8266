# 🔥 Fire Detection System — ESP8266

> Rapid-response fire and smoke detection using ESP8266, DHT11 temperature sensor, and MQ-2 smoke sensor with real-time alerts.

---

## 🎯 Aim

To design and develop an IoT-based **Fire Detection System** using the ESP8266 microcontroller, integrated with a DHT11 temperature sensor and an MQ-2 smoke sensor, ensuring **rapid detection of fire hazards** through real-time monitoring, audible/visual alarms, and optional WiFi-based notifications — thereby enhancing safety in homes, labs, and industrial environments.

**GitHub Repository:** `https://github.com/edwiee/FIRE-DETECTION-SYSTEM---ESP8266`

---

## 📋 Problem Statement

Fire accidents are among the most devastating incidents, causing significant loss of life and property. Traditional fire detection systems are often slow, expensive, or limited to single-parameter detection (smoke only). There is a critical need for a **low-cost, multi-parameter, IoT-enabled fire detection system** that can simultaneously monitor temperature and smoke levels, trigger immediate alerts, and be deployable in any environment.

---

## 🔭 Scope of Solution

| Area | Coverage |
|---|---|
| **Detection** | Temperature (DHT11) + Smoke/Gas (MQ-2) dual-parameter sensing |
| **Alerts** | Audible buzzer + Red/Green LED visual indicators |
| **Display** | 16×2 I2C LCD with real-time sensor readings |
| **Connectivity** | Optional WiFi (ESP8266 built-in) for remote notifications |
| **Threshold Control** | Configurable thresholds for both temp (°C) and smoke (ADC) |
| **User Interaction** | Push button to silence/acknowledge alerts |
| **Platform** | Suitable for homes, offices, labs, warehouses |
| **Scalability** | Can extend to MQTT/HTTP cloud dashboards |

---

## 🔧 Required Components

### Hardware

| Component | Specification | Quantity |
|---|---|---|
| ESP8266 NodeMCU | v1.0 / Lolin / WeMos D1 Mini | 1 |
| DHT11 Sensor | Temperature & Humidity | 1 |
| MQ-2 Sensor | Smoke, LPG, CO, Propane | 1 |
| Active Buzzer | 5V, 2300Hz | 1 |
| Red LED | 5mm, 2V | 1 |
| Green LED | 5mm, 2V | 1 |
| 16×2 LCD (I2C) | With PCF8574 I2C backpack | 1 |
| Push Button | Tactile, 6mm | 1 |
| Resistors | 220Ω (LEDs), 10kΩ (button) | 3 |
| Breadboard | 830-point | 1 |
| Jumper Wires | M-M, M-F | ~20 |
| USB Cable | Micro USB (for NodeMCU) | 1 |
| Power Supply | 5V / 1A USB or external | 1 |

### Software & IDE

| Tool | Version / Details |
|---|---|
| **Arduino IDE** | 2.x (Primary IDE) |
| **ESP8266 Board Package** | v3.1.2 — ESP8266 Community |
| **DHT Sensor Library** | Adafruit DHT Library v1.4.x |
| **LiquidCrystal_I2C** | Frank de Brabander v1.1.2 |
| **Simulation — Tinkercad** | https://www.tinkercad.com (Circuit + Code) |
| **Fritzing** | v0.9.10 (PCB / Gerber export) |
| **Serial Monitor** | Built into Arduino IDE (115200 baud) |

---

## 📐 Circuit Connections

| Sensor/Component | ESP8266 Pin | NodeMCU Label |
|---|---|---|
| DHT11 Data | GPIO 2 | D4 |
| MQ-2 Analog Out | A0 | A0 |
| Buzzer (+) | GPIO 12 | D6 |
| Red LED (+) | GPIO 13 | D7 |
| Green LED (+) | GPIO 14 | D5 |
| Button | GPIO 0 | D3 |
| LCD SDA | GPIO 4 | D2 |
| LCD SCL | GPIO 5 | D1 |
| VCC (3.3V) | 3.3V | 3V3 |
| VCC (5V) | VIN | VIN |
| GND | GND | GND |

---

## 🖼️ Simulated Circuit

- **Tinkercad Simulation:** [View Circuit](https://www.tinkercad.com/things/[YOUR_PROJECT_ID])
- **Fritzing Diagram:** See `/gerber/fire_detection_fritzing.fzz`
- **Gerber Files:** See `/gerber/` folder

---

## 🎬 Demo

- **Video Demo:** See `/demo/fire_detection_demo.mp4`
- **Screen Capture:** See `/demo/screenshots/`

---

## 📁 Repository Structure

```
fire-detection-esp8266/
├── fire_detection/
│   └── fire_detection.ino       # Main Arduino sketch
├── gerber/
│   ├── fire_detection.gbr       # Gerber top copper
│   ├── fire_detection_B.Cu.gbr  # Bottom copper
│   ├── fire_detection.drl       # Drill file
│   └── fire_detection_fritzing.fzz
├── demo/
│   ├── fire_detection_demo.mp4
│   └── screenshots/
│       ├── normal_state.png
│       └── alert_triggered.png
├── docs/
│   ├── circuit_diagram.png
│   └── component_list.pdf
└── README.md
```

---

## ⚙️ How It Works

1. **Initialization** — System starts, LCD shows "Fire Detection System v1.0", WiFi connects (if configured).
2. **Continuous Monitoring** — Every 1 second, DHT11 reads temperature/humidity and MQ-2 reads smoke/gas ADC level.
3. **Threshold Check**:
   - Temperature ≥ 50°C **OR** Smoke ADC ≥ 400 → **ALERT**
4. **Alert Sequence**:
   - Red LED turns ON, Green LED OFF
   - Buzzer pulses at 1000Hz
   - LCD displays "!! FIRE ALERT !!"
   - Serial monitor logs event with values
5. **Silence** — User presses the button to silence the buzzer while alert is acknowledged.
6. **Reset** — When conditions return to normal, system auto-resets to normal state.

---

## 📦 Library Installation

In Arduino IDE: **Tools → Manage Libraries** and search for:
- `DHT sensor library` by Adafruit
- `LiquidCrystal I2C` by Frank de Brabander

---

## 🚀 Upload Instructions

1. Install Arduino IDE 2.x
2. Add ESP8266 board URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Select Board: **NodeMCU 1.0 (ESP-12E Module)**
4. Select Port: Your COM port
5. Click **Upload**

---

## 📄 License

MIT License — Free to use, modify, and distribute.
