//#include <NewPing.h>
#include <Wire.h>   // incluye libreria bus I2C
#include <MechaQMC5883.h> // incluye libreria para magnetometro QMC5883L
#include <WiFi.h>
#include "FirebaseESP32.h"

#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define MAX_DISTANCE 300
#define WIFI_NETWORK "WIFI_VERA_2.4"
#define WIFI_PASSWORD "108021586"
#define FIREBASE_HOST "https://pruebassp-1d2ac-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "09cTeMqkdTOSg9llTHiNLTKVTFNCv2fHjqLw0ciL"

FirebaseData firebaseData;
int count = 0;
MechaQMC5883 qmc;
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA, SCL);  // inicializa bus I2C
  qmc.init();  //inicializa objeto
  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    //Serial.println("Connecting to WiFi...");
  }
  //Serial.println("Connected to the WiFi Network");
  //Serial.println("Wifi.localIP");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() {
  //Variables de ditancia y campo magnetico
  int x, y, z;
  int x2 = 0;
  int y2 = 0;
  //int distance;
  String cadenaX = "X: ";
  String cadenaY = "Y: ";
  for (int i = 0; i < 20; i++){
    qmc.read(&x,&y,&z); 
    x2 = x2 + x;
    y2 = y2 + y;
  }
  
  x2 = x2/20;
  y2 = y2/20;
  
  String aux = "data";
  aux += String(count);

  Firebase.setInt(firebaseData,aux+"/"+cadenaX,x2);
  Firebase.setInt(firebaseData,aux+"/"+cadenaY,y2);
  
  delay(2000);
  count++;
}

  /*JsonArray data = doc.createNestedArray(aux);
  serializeJson(doc, Serial);
  data.add(x);
  data.add(y);*/

  /*StaticJsonDocument<256> doc;
  JsonArray tupla = doc.to<JsonArray>();
  tupla.add(x);
  tupla.add(y);
  Firebase.setArray(firebaseData, tupla);*/
