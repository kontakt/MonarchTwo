/* ODU Monarch Two Sensor Package Code

    This code handles sensor data collection from a package of sensors
    onboard the Monarch Two suborbital rocket payload, and transfers them
    to a smartphone for data transmission to a groundstation.

    All code is provided under the BSD license. See license.txt.

    Authors:
      Connor Huffine

*/

/* Teensy pin connections
   SD Card
     CS    10
     MOSI  11
     MISO  12
     SCLK  13
   I2C (ADS1115, LSM9DS1, ITG3701)
     SCL   19
     SDA   18
   ADXL193 Self-Test 15
   SSR Control 14
   ADS1115 Connections
     A0  Solar Voltage
     A1  Solar Current Low
     A2  Solar Current High
     A3  ADXL193 Output
*/

//// Includes ////
#include "libraries/SparkFun_LSM9DS1/SparkFunLSM9DS1.h"   // 9DOF Sensor
#include "libraries/Adafruit_ADS1X15/Adafruit_ADS1015.h"  // 16 Bit ADC
#include "libraries/ITG3701/ITG3701.h"                    // ITG3701
#include <Wire.h>                                         // I2C
#include <SPI.h>                                          // SPI

//// Object declarations ////
LSM9DS1           IMU;    // Declare 9DOF object  (addr 0x1E, 0x6B)
Adafruit_ADS1115  ADS;    // Declare ADC object   (addr 0x48)
ITG3701 hDPS(ITG3701_ADDRESS);  // high DPS

//// Defines ////
#define LSM9DS1_M   0x1E
#define LSM9DS1_AG  0x6B
#define ITG3701_ADDRESS 0x68

void setup() {
  Serial.begin(19200); // Intialize serial line
  Wire.begin();
  IMU.settings.device.commInterface = IMU_MODE_I2C;
  IMU.settings.device.mAddress      = LSM9DS1_M;
  IMU.settings.device.agAddress     = LSM9DS1_AG;
  hDPS.begin(ITG3701::GFS_4000DPS, ITG3701::GODR_380Hz, ITG3701::GBW_highest);  // Initialize Gyro
  IMU.begin();          // Initialize the 9DOF
  ADS.begin();          // Initialzie the ADC
}

void loop() {
  Serial.println();
  Serial.println("=== 9DOF ===");
  Serial.println("== Gyroscope ==");
  IMU.readGyro();
  Serial.print(IMU.gx);
  Serial.print(", ");
  Serial.print(IMU.gy);
  Serial.print(", ");
  Serial.println(IMU.gz);
  Serial.println("== Accelerometer ==");
  IMU.readAccel();
  Serial.print(IMU.ax);
  Serial.print(", ");
  Serial.print(IMU.ay);
  Serial.print(", ");
  Serial.println(IMU.az);
  Serial.println("== Magnetometer ==");
  IMU.readMag();
  Serial.print(IMU.mx);
  Serial.print(", ");
  Serial.print(IMU.my);
  Serial.print(", ");
  Serial.println(IMU.mz);
  Serial.println("=== ADC ===");
  Serial.println("== Solar Voltage ==");
  Serial.println(ADS.readADC_SingleEnded(0));
  Serial.println("== Solar Current 1 ==");
  Serial.println(ADS.readADC_SingleEnded(1));
  Serial.println("== Solar Current 2 ==");
  Serial.println(ADS.readADC_SingleEnded(2));
  Serial.println("== High G Accelerometer ==");
  Serial.println(ADS.readADC_SingleEnded(3));
  Serial.println("=== High DPS gyro ===");
  hDPS.updateGyro();
  Serial.print(hDPS.x);
  Serial.print(", ");
  Serial.print(hDPS.y);
  Serial.print(", ");
  Serial.println(hDPS.z);
  delay(250);
}
