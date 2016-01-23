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

#include "libraries/SparkFun_LSM9DS1/SparkFunLSM9DS1.h"   // 9DOF Sensor
#include "libraries/Adafruit_ADS1X15/Adafruit_ADS1015.h"  // ADC
#include "libraries/ITG3701/ITG3701.h"                    // ITG3701

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
