#include "heltec.h" 
#include "images.h"
#include <WiFi.h>
#include "FirebaseESP32.h"

#define BAND    915E6
#define WIFI_NETWORK "Wifitelsur_VERA"
#define WIFI_PASSWORD "108021586"
#define FIREBASE_HOST "https://smart-parking-9e55f-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "qTB5hGhV7e6hRUZhSVhAusBdNiqKAPknHaXuFWWN"

FirebaseData firebaseData;

String parking = "Parking: ";

String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  LoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.begin(9600);
  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi Network");
  Serial.println("Wifi.localIP");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  int size_packet = packet.length();
  int cont = 0;
  String slot = "";
  String value = "";
  
  Serial.println(packet);
  if (size_packet > 1) {
    while(packet[cont] != ':'){
     cadena_1 += packet[cont];
     cont += 1;
     Serial.println(cont);
    }
  }
  value = packet.substring(cont+1);
  
  Serial.println(slot + ":");
  Serial.println(value);
  
  Firebase.setString(firebaseData,cadena_1,cadena_2);
  Firebase.end(firebaseData);
  delay(250);
