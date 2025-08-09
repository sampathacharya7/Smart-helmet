🪖 Smart Helmet Accident Alert System
📌 Introduction
The Smart Helmet Accident Alert System is an IoT + Android-based safety solution designed to improve rider safety in case of a vehicular accident.

It integrates the MPU6050 motion sensor module (accelerometer + gyroscope) with an ESP32 microcontroller, mounted inside the helmet. The system continuously monitors motion, detects sudden impacts or abnormal acceleration patterns, and automatically triggers an accident alert process.

How it Works:
Accident Detection — The ESP32 uses MPU6050 data to detect crashes based on acceleration thresholds.

Bluetooth Communication — If an impact is detected, an emergency signal is sent to the rider’s smartphone via Bluetooth.

GPS Location & SMS Alert — The smartphone app retrieves the current GPS location and sends an SMS alert to a predefined guardian.

Guardian Notification — A dedicated Guardian App plays a siren, vibrates the phone, and shows a popup alert when it receives the SMS.

This dual-app system ensures quick accident reporting, enabling faster emergency response and potentially saving lives.

🎯 Objectives
Accurate Crash Detection — Use MPU6050 sensor + ESP32 to precisely detect high-impact events.

Automated SMS Alerts — Send GPS location via SMS to a predefined guardian’s phone.

Immediate Guardian Notification — Trigger siren sound, vibration, and popup through the Guardian app.

🏗 System Architecture
Hardware Components:

ESP32 Development Board

MPU6050 Accelerometer + Gyroscope

Li-Ion Battery

Power Switch

TP4056 Charging Module

Helmet (prototype integration)

Software Tools:

Arduino IDE – ESP32 firmware programming

Android Studio – Smart Helmet & Guardian app development

⚙️ Implementation
1️⃣ ESP32 Firmware
Reads real-time acceleration & gyroscope data from MPU6050 via I²C.

Computes acceleration magnitude and compares with a preset threshold.

On exceeding threshold (impact detected), sends "ACCIDENT DETECTED" via Bluetooth to paired smartphone.

Sample Code:
cpp
#include <Wire.h>
#include "BluetoothSerial.h"

#define MPU6050_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43
#define PWR_MGMT_1   0x6B

#define ACCEL_SCALE 16384.0  // ±2g
#define GYRO_SCALE 131.0     // ±250 deg/s

BluetoothSerial SerialBT;
const int LED_BUILTIN = 2;

void setup () {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("Helmet-Alert");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("System ON. Monitoring started...");
}

void loop () {
  float ax = readWord(ACCEL_XOUT_H) / ACCEL_SCALE;
  float ay = readWord(ACCEL_XOUT_H + 2) / ACCEL_SCALE;
  float az = readWord(ACCEL_XOUT_H + 4) / ACCEL_SCALE;
  
  float accMagSq = ax * ax + ay * ay + az * az;
  Serial.printf("AX=%.2f AY=%.2f AZ=%.2f | Acc²=%.2f\n", ax, ay, az, accMagSq);

  if (accMagSq > 7.0) { // ≈ 5g impact
    Serial.println("ACCIDENT DETECTED!");
    SerialBT.println("ACCIDENT DETECTED!");
    delay(5000); // Avoid repeated alerts
  }
  delay(200);
}

int16_t readWord(int reg) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 2, true);
  return (Wire.read() << 8) | Wire.read();
}
Note: The threshold 7.0 can be adjusted based on sensitivity needs.

2️⃣ Smart Helmet Android App
Bluetooth Connection to "Helmet-Alert" device (ESP32).

Listens for "ACCIDENT DETECTED" messages in real-time.

On detection, retrieves GPS location and sends an SMS alert with Google Maps link to the guardian.

Includes Settings screen to update guardian contact.

Monitoring starts automatically when connected.

3️⃣ Guardian Android App
Listens for relevant SMS alerts using BroadcastReceiver.

If message contains "ACCIDENT DETECTED", it:

Plays siren sound.

Vibrates the phone.

Displays alert popup or toast notification.

Optionally requests DND override permission to sound alert even in silent mode.

🔄 Workflow Summary
System Power-On – ESP32 starts reading MPU6050 data.

Impact Detection – If acceleration exceeds threshold, triggers accident alert.

Bluetooth Alert – ESP32 sends "ACCIDENT DETECTED" to Android app.

GPS & SMS – Smart Helmet app sends location SMS to guardian.

Guardian Response – Guardian app detects SMS and triggers loud alarm.

🛠️ Technology Stack
Hardware: ESP32, MPU6050, Li-Ion, TP4056

Firmware: Arduino (C/C++)

Mobile Apps: Android (Java/Kotlin)

Connectivity: Bluetooth Serial

Services: GPS, SMS API

🚀 Future Improvements
Link directly with local emergency services.

Add fall detection algorithms for bicycles and scooters.

Cloud-based crash data storage for analytics.

