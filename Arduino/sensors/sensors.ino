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
#include <SdFat.h>                                        // SD Card

//// Object declarations ////
LSM9DS1           IMU;    // Declare 9DOF object  (addr 0x1E, 0x6B)
Adafruit_ADS1115  ADS;    // Declare ADC object   (addr 0x48)
ITG3701 hDPS(ITG3701_ADDRESS);  // high DPS
SdFat SD;                 // SD Card
SdFile file;

//// Defines ////
#define LSM9DS1_M   0x1E
#define LSM9DS1_AG  0x6B
#define ITG3701_ADDRESS 0x68
#define SD_CS_PIN 10
#define FILE_BASE_NAME "Data"

// Write data header.
void writeHeader() {
  file.print(F("ms"));
  file.print(F(",GX"));
  file.print(F(",GY"));
  file.print(F(",GZ"));
  file.print(F(",AX"));
  file.print(F(",AY"));
  file.print(F(",AZ"));
  file.print(F(",MX"));
  file.print(F(",MY"));
  file.print(F(",MZ"));
  file.print(F(",SolarV"));
  file.print(F(",SolarI1"));
  file.print(F(",SolarI2"));
  file.print(F(",HAZ"));
  file.print(F(",HGX"));
  file.print(F(",HGY"));
  file.print(F(",HGZ"));
  file.println();
}

const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[13] = FILE_BASE_NAME "00.csv";
unsigned long last_time = 0;
unsigned long last_tx = 0;

void setup() {
  Serial.begin(9600); // Intialize serial line
  Serial.println("Loading");
  pinMode(10, OUTPUT);
  pinMode(14, OUTPUT);
  SD.begin(SD_CS_PIN, SPI_FULL_SPEED);
  Wire.begin();
  IMU.settings.device.commInterface = IMU_MODE_I2C;
  IMU.settings.device.mAddress      = LSM9DS1_M;
  IMU.settings.device.agAddress     = LSM9DS1_AG;
  hDPS.begin(ITG3701::GFS_4000DPS, ITG3701::GODR_380Hz, ITG3701::GBW_highest);  // Initialize Gyro
  IMU.begin();          // Initialize the 9DOF
  ADS.begin();          // Initialzie the ADC

  while (SD.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    }
  }
  file.open(fileName, O_CREAT | O_WRITE | O_EXCL);
  
  // Write data header.
  writeHeader();
}

void loop() {
  int data[16];

  digitalWrite(14, LOW);
  
  IMU.readGyro();
  IMU.readAccel();
  IMU.readMag();
  hDPS.updateGyro();
  
  data[0] = IMU.gx;
  data[1] = IMU.gy;
  data[2] = IMU.gz;
  
  data[3] = IMU.ax;
  data[4] = IMU.ay;
  data[5] = IMU.az;

  data[6] = IMU.mx;
  data[7] = IMU.my;
  data[8] = IMU.mz;

  data[9] = ADS.readADC_SingleEnded(0);
  
  digitalWrite(14, HIGH);
  
  data[10] = ADS.readADC_SingleEnded(1);
  data[11] = ADS.readADC_SingleEnded(2);
  data[12] = ADS.readADC_SingleEnded(3);

  data[13] = hDPS.x;
  data[14] = hDPS.y;
  data[15] = hDPS.z;

//  Serial.println();
//  Serial.println("Data");
//  Serial.println("=== 9DOF ===");
//  Serial.println("== Gyroscope ==");
//  IMU.readGyro();
//  Serial.print(IMU.gx);
//  Serial.print(", ");
//  Serial.print(IMU.gy);
//  Serial.print(", ");
//  Serial.println(IMU.gz);
//  Serial.println("== Accelerometer ==");
//  IMU.readAccel();
//  Serial.print(IMU.ax);
//  Serial.print(", ");
//  Serial.print(IMU.ay);
//  Serial.print(", ");
//  Serial.println(IMU.az);
//  Serial.println("== Magnetometer ==");
//  IMU.readMag();
//  Serial.print(IMU.mx);
//  Serial.print(", ");
//  Serial.print(IMU.my);
//  Serial.print(", ");
//  Serial.println(IMU.mz);
//  Serial.println("=== ADC ===");
//  Serial.println("== Solar Voltage ==");
//  Serial.println(ADS.readADC_SingleEnded(0));
//  Serial.println("== Solar Current 1 ==");
//  Serial.println(ADS.readADC_SingleEnded(1));
//  Serial.println("== Solar Current 2 ==");
//  Serial.println(ADS.readADC_SingleEnded(2));
//  Serial.println("== High G Accelerometer ==");
//  Serial.println(ADS.readADC_SingleEnded(3));
//  Serial.println("=== High DPS gyro ===");
//  hDPS.updateGyro();
//  Serial.print(hDPS.x);
//  Serial.print(", ");
//  Serial.print(hDPS.y);
//  Serial.print(", ");
//  Serial.println(hDPS.z);
//  delay(250);

  last_time = millis();
  file.open(fileName, FILE_WRITE);
  file.print(last_time);
  for (uint8_t i = 0; i < 16; i++) {
    file.write(',');
    file.print(data[i]);
  }
  file.println();
  file.close();
  if(millis() - last_tx > 5000){ 
    Serial.write((uint8_t*)data, sizeof(data));
    last_tx = millis();
  }
  while(millis() - last_time < 50);
}
