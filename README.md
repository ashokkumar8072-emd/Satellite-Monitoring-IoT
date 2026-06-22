# 🛰️ Satellite Health Monitoring System

## 📖 Project Overview

The Satellite Health Monitoring System is an IoT-based embedded system developed using ESP32 for real-time monitoring of critical satellite health parameters. The system collects environmental and motion data using sensors, processes the information, displays it on an OLED screen, and uploads telemetry data to the ThingSpeak cloud platform for remote monitoring and analysis.

This project simulates a basic satellite telemetry and health monitoring architecture similar to those used in aerospace systems.

---

## 🚀 Features

* Real-Time Temperature Monitoring
* Real-Time Humidity Monitoring
* Accelerometer Monitoring (X, Y, Z)
* Gyroscope Monitoring (X, Y, Z)
* Pitch and Roll Calculation
* OLED-Based Health Dashboard
* Fault Detection and Warning System
* Buzzer Alert for Abnormal Conditions
* Wi-Fi Connectivity using ESP32
* Cloud Telemetry using ThingSpeak
* Real-Time Data Visualization
* Wokwi Simulation Support

---

## 🛠 Hardware Components

| Component    | Purpose                           |
| ------------ | --------------------------------- |
| ESP32        | Main Controller                   |
| DHT11 Sensor | Temperature & Humidity Monitoring |
| MPU6050      | Accelerometer & Gyroscope         |
| SSD1306 OLED | Live Health Display               |
| Buzzer       | Fault Alert Indication            |
| Jumper Wires | Connections                       |
| USB Cable    | Programming & Power               |

---

## 📊 Parameters Monitored

### Environmental Parameters

* Temperature (°C)
* Humidity (%)

### Motion Parameters

* Acceleration X
* Acceleration Y
* Acceleration Z

### Orientation Parameters

* Gyroscope X
* Gyroscope Y
* Gyroscope Z
* Pitch Angle
* Roll Angle

### Health Status

* HEALTHY
* WARNING

---

## ☁️ ThingSpeak Cloud Dashboard

The collected telemetry data is uploaded to ThingSpeak for remote monitoring and visualization.

### Channel Fields

| Field   | Parameter      |
| ------- | -------------- |
| Field 1 | Temperature    |
| Field 2 | Humidity       |
| Field 3 | Acceleration X |
| Field 4 | Acceleration Y |
| Field 5 | Acceleration Z |
| Field 6 | Gyroscope X    |
| Field 7 | Gyroscope Y    |
| Field 8 | Gyroscope Z    |

### Dashboard Link

https://thingspeak.mathworks.com/channels/3413041/private_show

---

## 🖥 OLED Dashboard

### Screen 1

```text
Temp : XX.X C
Hum  : XX %

Pitch: XX.X
Roll : XX.X
```

### Screen 2

```text
SATELLITE HEALTH

STATUS:
HEALTHY
```

or

```text
SATELLITE HEALTH

STATUS:
WARNING
```

---

## 💻 Software & Tools

* Arduino IDE
* ESP32 Board Package
* ThingSpeak Cloud Platform
* GitHub
* Wokwi Simulator

---

## 📂 Project Structure

```text
Satellite-Health-Monitoring-System/
│
├── sketch.ino
├── diagram.json
├── libraries.txt
├── wokwi-project.txt
├── README.md
```

---

## ⚙️ Working Principle

1. DHT11 measures temperature and humidity.
2. MPU6050 measures acceleration and angular velocity.
3. ESP32 processes sensor data.
4. Pitch and Roll angles are calculated.
5. Health status is determined based on predefined thresholds.
6. Data is displayed on the OLED screen.
7. Telemetry data is uploaded to ThingSpeak.
8. Buzzer activates when abnormal conditions are detected.

---

## 🔮 Future Enhancements

* Battery Voltage Monitoring
* Current Consumption Monitoring
* GPS Integration
* Cloud-Based Data Analytics
* Mobile Application
* AI-Based Fault Prediction
* Satellite Communication Simulation
* Advanced Telemetry Dashboard

---

## 👨‍💻 Author

Ashok Kumar

Electronics and Communication Engineering (ECE)

Ramco institute of technology

Project Domain: Embedded Systems • IoT • Satellite Telemetry • ESP32
