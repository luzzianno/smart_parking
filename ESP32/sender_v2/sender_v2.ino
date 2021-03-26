#include <NewPing.h>
#include <Wire.h>   // incluye libreria bus I2C
#include <MechaQMC5883.h> // incluye libreria para magnetometro QMC5883L
#include "heltec.h"

#define BAND    915E6  //Banda red LoRa
#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define MAX_DISTANCE 300

String rsii = "RSII --";
String packSize = "--";
String packet ;


MechaQMC5883 qmc;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(9600);
  Wire.begin(SDA, SCL);  // inicializa bus I2C
  qmc.init();  //inicializa objeto

}

void loop() {
  //Variables de ditancia y campo magnetico
  int x, y, z, distance;
  String state;
  int x2 = 0;
  int y2 = 0;
  int z2 = 0;
  int distance2 = 0;

  // Lectura de valores
  for (int i = 0; i < 10; i++){
    qmc.read(&x,&y,&z); 
    distance = sonar.ping_cm();
    x2 = x2 + x;
    y2 = y2 + y;
    z2 = z2 + z;
    distance2 = distance2 + distance;
  }
  
  //Normalizacion de valores
  x2 = x2/10;
  y2 = y2/10;
  z2 = z2/10;
  distance2 = distance2/10;

  //Evaluacion de ocupancia del slot de estacionamiento
  if ((distance < 50) && (10 < x < 50) && (10 < y < 50) && (10 < z < 50)){
    state = "occupied";
  }
  else{
    state = "unoccupied";
  }

  //Envio de packete
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("Ocupacion: ");
  LoRa.print(String(distance2));
  LoRa.endPacket();
  delay(500);
}
