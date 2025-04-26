# Solar Monitoring System

**Developed By:** Bootgrow Solutions LLP  

---

## 📋 Project Overview

The Solar Monitoring System is a robust, industrial-grade IoT device designed to monitor key environmental parameters and solar inverter metrics in real time.

It enables efficient operational oversight, remote maintenance through OTA updates, and reliable data transmission over WiFi or 4G LTE networks.

---

## 🛠 Features

- **Multi-Sensor Integration:**
  - Temperature and Humidity (DHT22)
  - Solar Irradiance (350-1100nm spectrum)
  - Wind Speed and Direction (Model 6410)
  - Dust Level Monitoring

- **Inverter Monitoring:**
  - RS485 Communication over RJ45 Connector
  - Power Output, Voltage, Current, Battery Status Monitoring

- **Connectivity:**
  - WiFi First Priority
  - Automatic 4G LTE Fallback (SIM7600E-H Module)

- **User Interface:**
  - Real-Time Data Display on LCD (20x4 I2C)
  - OTA (Over-The-Air) Firmware Update Support

- **Industrial Design:**
  - Compact, IP65-Grade Enclosure
  - Minimal Cable Access: Power, Sensors, RJ45 Inverter, SIM Tray

---

## 🔧 Hardware Requirements

- ESP32 Development Board
- SIM7600E-H 4G LTE Module
- DHT22 Temperature and Humidity Sensor
- Solar Irradiance Sensor (Analog)
- Wind Speed and Direction Sensor (Model 6410)
- Dust Sensor (Analog)
- Buck Converter (12V/24V to 5V DC)
- RS485 Transceiver (MAX485 or SP3485)
- LCD 20x4 I2C Display
- RJ45 Connector for Inverter Communication
- SIM Tray Holder for External Access
- Waterproof IP65 Enclosure

---

## 📡 API Endpoints Used

- `POST /api/sensors/send-sensor-data`  
- `POST /api/inverter/send-inverter-data`

**Data format:** JSON over HTTP

---

## 📈 Future Expansion

- Integration with cloud dashboards (Grafana, ThingsBoard, Custom Web Portal)
- Predictive Maintenance using collected environmental data
- GPS tracking integration for mobile solar units

---

## 📝 License

This project is proprietary to **Bootgrow Solutions LLP** and developed for **Takyon Networks Private Limited**.  
Unauthorized reproduction, distribution, or deployment is prohibited.

---
