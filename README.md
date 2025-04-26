# Solar Monitoring System (ESP32 based)

This project is a **Solar Monitoring System** built on ESP32 designed for industrial deployment near Solar Inverters.  
It reads **environmental sensor data** and **solar inverter operational data**, sends it to a **remote API server**, displays it on an **LCD**, and can push **critical SMS alerts** in case of inverter or battery issues.

The system features:
- **Auto Baud Rate Detection** for inverter RS485 communication.
- **Flash Memory Save/Load** to avoid repeated detection.
- **WiFi + GSM fallback** internet connection.
- **Secure OTA (Over-the-Air) Firmware Update**.
- **LCD Auto-Scrolling Pages** to show live data.
- **Critical Fault Detection and SMS Alerts**.

---

## 🔥 Available Versions

| Version | Description |
|:--------|:------------|
| **Static Version** (`Solar_Monitoring_System_Final_Static/`) | OTA Hostname and Password are **fixed manually** inside the code. |
| **Dynamic Version** (`Solar_Monitoring_System_Final_Dynamic/`) | OTA Hostname and Password are **generated automatically** based on the ESP32 **chip unique ID** at runtime. |

✅ Both versions fully support Flash Save, Auto Baud, LCD, GSM/WiFi, API Push, and SMS Alerts.

---

## 📂 Project Folder Structure

```plaintext
Solar_Monitoring_System/
├── Solar_Monitoring_System_Final_Static/
│    ├── Solar_Monitoring_System_Final_Static.ino
│    ├── InverterSettings.h
│    ├── AutoBaudScanner.h
│    ├── LCDManager.h
│    ├── RS485Reader.h
├── Solar_Monitoring_System_Final_Dynamic/
│    ├── (Similar structure with dynamic OTA setup)
└── README.md
```

---

## 📦 Arduino Libraries Required

You must install these libraries from Arduino IDE Library Manager:

| Library | Purpose |
|:--------|:--------|
| TinyGSM | GSM module (SIM800/SIM900/4G) handling |
| WiFi.h | Native ESP32 WiFi |
| DHT Sensor Library | Humidity/Temperature Sensor (DHT22) |
| LiquidCrystal_I2C | LCD Display over I2C |
| HTTPClient | Sending API requests |
| ArduinoOTA | Secure OTA updates |
| Preferences | Flash memory handling |
| HardwareSerial | Custom RS485 UART handling |

✅ These libraries are free and available directly from Arduino IDE Library Manager.

---

## 🚀 How to Deploy

1. Install ESP32 board in Arduino IDE (if not already).
2. Install the required libraries listed above.
3. Open the `.ino` file (Static or Dynamic version).
4. Update your WiFi credentials, GSM APN, and mobile number for SMS alerts.
5. Compile and upload firmware to ESP32.
6. Device will:
   - Connect to WiFi (fallback to GSM if WiFi fails)
   - Perform Auto Baud detection if no Flash settings found
   - Start sending data to API and scrolling LCD
   - Send Critical SMS on Inverter Fault or Low Battery
7. Perform OTA firmware updates securely as needed.

---

## 🛠 Future Enhancements (Planned)

- Integrate real-time RS485 Modbus inverter data reading once inverter register map is provided.
- Add external SD card logging for offline history.
- Integrate WiFi Captive Portal for field WiFi configuration without flashing.

---

# 📢 Notes
- For **Static OTA Version**, Hostname and Password are manually set inside firmware.
- For **Dynamic OTA Version**, Hostname and Password are automatically generated from ESP32 Chip Unique ID.

---

# 🔥 Project Status
✅ Stable and field deployable (Pilot Ready).  
✅ Future-proof architecture for easy scale and upgrades.

---

# 📬 Support and Contact
Developed by **Bootgrow Solutions LLP** 
For technical queries or support, please raise issues in the repository or contact the development team directly.

---
