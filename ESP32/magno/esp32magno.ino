#include <Wire.h>
#include "heltec.h"
#include <MechaQMC5883.h>

MechaQMC5883 qmc;

void setup() {
  Heltec.begin(true, false, true);
  //Wire.begin(SDA_OLED, SCL_OLED); //Scan OLED's I2C address via I2C0
  Wire.begin(SDA, SCL); 
  qmc.init();
  //qmc.setMode(Mode_Continuous,ODR_200Hz,RNG_2G,OSR_256);
}

void loop() {
  int x, y, z;
  int azimuth;
  //float azimuth; //is supporting float too
  qmc.read(&x, &y, &z,&azimuth);
  //azimuth = qmc.azimuth(&y,&x);//you can get custom azimuth
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.print(z);
  Serial.print(" a: ");
  Serial.print(azimuth);
  Serial.println();
  delay(100);
}
