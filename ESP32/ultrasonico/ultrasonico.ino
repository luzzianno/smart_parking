#include "heltec.h"
#define BAND    915E6
#define pinTrigger 12
#define pinEcho 13

// Declaracion de funciones
float lecturaSimple();
float calcularDistancia();
void sonarBegin(byte trig ,byte echo);

// Declaracion de variables
float distancia;


void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(9600);
  sonarBegin(pinTrigger,pinEcho);
}

void loop() {

  distancia = calcularDistancia();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(40);

}

// Implementacion de funciones

void sonarBegin(byte trig ,byte echo) {

  #define divisor 58.0
  #define intervaloMedida 5
  #define qtdMedidas 20
  
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  digitalWrite(trig, LOW);
  delayMicroseconds(500);

}

float calcularDistancia() {

  float lecturaSum = 0;
  float resultado = 0;
  
  for (int i = 0; i < qtdMedidas; i++) {

    delay(intervaloMedida);
    lecturaSum += lecturaSimple();

  }

  resultado = (float) lecturaSum / qtdMedidas;
  resultado = resultado + 2.2;
  
  return resultado;

}

float lecturaSimple() {

  long duracion = 0; 
  float resultado = 0;
  
  digitalWrite(pinTrigger, HIGH); 

  delayMicroseconds(10); 
  digitalWrite(pinTrigger, LOW);

  duracion = pulseIn(pinEcho, HIGH);

  resultado = ((float) duracion / divisor);
  
  return resultado;

}
