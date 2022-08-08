#include <NewPing.h>
#include <Wire.h>   // incluye libreria bus I2C
#include <MechaQMC5883.h> // incluye libreria para magnetometro QMC5883L
#include <WiFi.h>
#include "heltec.h"
//HOLA DILUCIAN
#define BAND    915E6  //Banda red LoRa
#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define WIFI_NETWORK "WIFI_VERA_2.4"
#define WIFI_PASSWORD "108021586"
#define TIME 10
#define uS_to_S 1000000
#define MAX_MULTIPLIER 6

MechaQMC5883 qmc;
RTC_DATA_ATTR int x_global;
RTC_DATA_ATTR int y_global;
int z_global;
RTC_DATA_ATTR int state_global = 0;
RTC_DATA_ATTR char multiplier = 1;
RTC_DATA_ATTR bool calibrate = false;
NewPing sonar(TRIGGER_PIN, ECHO_PIN);

void setup() {
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Wire.begin(SDA, SCL);  // inicializa bus I2C
  qmc.init();  //inicializa objeto campo magnetico
  if(calibrate == false){
    int x = 0;
    int y = 0;
    delay(10000);
    for (int i = 0; i < 100; i++){
        qmc.read(&x_global,&y_global,&z_global); 
        x += x_global;
        y += y_global;
      }
    x_global = x/100;
    y_global = y/100;
    calibrate = true;
  }
}

void loop() {
  
  Serial.print("X_blobal: ");
  Serial.println(x_global);
  Serial.print("Y_blobal: ");
  Serial.println(y_global);
  Serial.print("Estado: ");
  Serial.println(state_global);
  int x, y, z, distance, state;
  int x2 = 0;
  int y2 = 0;
  int distance2 = 0;
  for (int i = 0; i < 20; i++){
    qmc.read(&x,&y,&z); 
    x2 = x2 + x;
    y2 = y2 + y;
    distance = sonar.ping_cm();
    distance2 += distance;
    delay(5);
  }
  
  x2 = x2/20;
  y2 = y2/20;
  distance2 = distance2/20;

  if(distance2 < 27){
    if((abs(x_global - x2) > 150) || (abs(y_global - y2) > 150)){
      state = 1;
    }
    else{
      state = 0;
    }
  }
  else{
    state = 0;
  }
  Serial.print("x2: ");
  Serial.println(x2);
  Serial.print("y2: ");
  Serial.println(y2);
  Serial.print("distancia: ");
  Serial.println(distance2);
  Serial.print("Estado2: ");
  Serial.println(state);
  if(state_global != state){
    state_global = state;
    multiplier = 1;
    LoRa.beginPacket();
    LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("slot_1a:");
    LoRa.print(String(state));
    LoRa.endPacket();
  }
  else{
    multiplier = constrain(multiplier+1,1,MAX_MULTIPLIER);
  }
  esp_sleep_enable_timer_wakeup(TIME * multiplier * uS_to_S);
  esp_deep_sleep_start();
  
  delay(2000);
}
