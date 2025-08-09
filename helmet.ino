#include <Wire.h>
#include "BluetoothSerial.h"

#define MPU6050_ADDR 0x68

#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define PWR_MGMT_1 0x6B

#define ACCEL_SCALE 16384.0 // ±2g
#define GYRO_SCALE 131.0 // ±250 deg/s

BluetoothSerial SerialBT;
const int LED_BUILTIN = 2;

void setup () {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("Helmet-Alert");

  pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate system is active

  // Wake up MPU6050
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

  float gx = readWord(GYRO_XOUT_H) / GYRO_SCALE;
  float gy = readWord(GYRO_XOUT_H + 2) / GYRO_SCALE;
  float gz = readWord(GYRO_XOUT_H + 4) / GYRO_SCALE;

  float accMagSq = ax * ax + ay * ay + az * az;

  Serial.printf("AX=%.2f AY=%.2f AZ=%.2f | GZ=%.2f | Acc²=%.2f\n", ax, ay, az, gz, accMagSq);

  if (accMagSq > 7.0) { 
    Serial.println("ACCIDENT DETECTED!");
    SerialBT.println("ACCIDENT DETECTED!");
    delay (5000); // Prevent spamming
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
