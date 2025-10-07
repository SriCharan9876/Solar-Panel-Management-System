# Solar Panel Management System  
**Board:** ESP32-DevKit-C-V4  
**Simulation Link:** [Wokwi Project](https://wokwi.com/projects/421426846140465153)  

---

## 🧠 Introduction
The Solar Panel Management System uses an ESP32 microcontroller to **automatically orient a solar panel** toward the brightest light source using an **LDR sensor**.  
It also monitors **temperature** and **humidity** via a DHT22 sensor and sends real-time data to the **Blynk IoT platform** for remote control and visualization.

---

## ⚙️ Components
1. ESP32-DevKit-C-V4  
2. DHT22 Temperature & Humidity Sensor  
3. LDR (Light Dependent Resistor)  
4. Servo Motor  
5. LED  
6. OLED Display (SSD1306)  
7. Push Button  
8. Jumper Wires  
9. Power Supply  
10. Wi-Fi Module  

---

## 🚩 Problem Statement
Traditional solar panels require **manual orientation** to maximize sunlight exposure. This limits efficiency and may cause damage during **extreme weather** conditions.

---

## 💡 Proposed Solution
- Automatically adjust solar panel orientation to maximize light intensity.  
- Monitor temperature and humidity for system protection.  
- Use **Blynk** for remote control and real-time monitoring.  
- Provide both **manual scan** (via push button) and **auto scan** (every 2 hours).

---

## 🧩 Key Functionalities
| Feature | Description |
|----------|--------------|
| ☀️ **Auto Scan** | Every 2 hours, automatically scans angles (0–180°) to find the best illumination. |
| 🖲️ **Manual Scan** | User can press a button or trigger it from Blynk to reorient the panel. |
| 🌦️ **Safety Checks** | Stops operation if temperature or humidity exceed safe thresholds. |
| 📱 **Blynk Integration** | Displays temperature, humidity, illumination, and system power status remotely. |
| 🖥️ **OLED Display** | Shows real-time readings and system messages. |

---

## 🧰 Libraries Used
- `WiFi.h`  
- `BlynkSimpleEsp32.h`  
- `ESP32Servo.h`  
- `DHT.h`  
- `Adafruit_GFX.h`  
- `Adafruit_SSD1306.h`  

---

## 🔗 Blynk Virtual Pins
| Virtual Pin | Function |
|--------------|-----------|
| `V0` | Orientation Button |
| `V1` | Power Status |
| `V2` | Temperature |
| `V3` | Humidity |
| `V4` | Illumination |

---

