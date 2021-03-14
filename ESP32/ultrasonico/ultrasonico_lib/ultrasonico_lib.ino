#include <NewPing.h>
#include <Wire.h>   // incluye libreria bus I2C
#include <MechaQMC5883.h> // incluye libreria para magnetometro QMC5883L
#include "heltec.h"

#define BAND    915E6  //Banda red LoRa
#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define MAX_DISTANCE 200

MechaQMC5883 qmc;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(9600);
  Wire.begin(SDA, SCL);  // inicializa bus I2C
  qmc.init();  //inicializa objeto

}

void loop() {
  int x,y,z,acimut;   // variables para los 3 ejes y acimut
  qmc.read(&x,&y,&z,&acimut); // funcion para leer valores y asignar a variables
  
  Serial.print("Ping: ");
  int distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println("cm");
  Serial.print("x:");   // muestra texto x:
  Serial.print(x);    // muestra valor de variable x
  Serial.print("\t");   // espacio de tabulacion
  Serial.print("y:");   // muestra y:
  Serial.print(y);    // muestra valor de variable y
  Serial.print("\t");   // espacio de tabulacion
  Serial.print("z:");   // muestra texto z:
  Serial.print(z);    // muestra valor de variable z
  Serial.print("\t");   // espacio de tabulacion
  Serial.print("a:");   // muestra texto a:
  Serial.println(acimut); // muestra valor de variable acimut
  delay(250);

}
