#define DHTTYPE DHT11  // используемый датчик, DHT11 или DHT22

#define mosPIN 2
#define sensVCC 3
#define sensDATA 4
#define sensGND 5


#include "DHT.h"
DHT dht(sensDATA, DHTTYPE);

unsigned long readTimer;
float hum;
int pot, minHumid, maxHumid;
bool isActive;

void setup() {
  pinMode(mosPIN, OUTPUT);
  pinMode(sensVCC, OUTPUT);
  pinMode(sensGND, OUTPUT);

  pinMode(sensDATA, INPUT_PULLUP);
  digitalWrite(mosPIN, 0);
  digitalWrite(sensVCC, 1);
  digitalWrite(sensGND, 0);

  Serial.begin(9600);
  dht.begin();
  delay(1000);
  hum = dht.readHumidity();
  isActive = false;
}

void loop() {
  pot = 40;   // перевод в диапазон примерно от 0 до 100
  minHumid = 30;
  maxHumid = 40;

  // подобие петли гистеезиса, чтоы избежать постоянных вкл-выкл с интеррвалом секунд в 10
  if(!isActive && minHumid > hum ) {
    digitalWrite(mosPIN, 1);       // врубить всё
    isActive = true;
  }
  
  if(isActive && maxHumid < hum ) {
    digitalWrite(mosPIN, 0);       // вырубить всё
    isActive = false;
  }
  
  
//  if (pot > hum) {                 // если установка больше текущей влажности
//    digitalWrite(mosPIN, 1);       // врубить всё
//
//  } else {                         // если нет
//    digitalWrite(mosPIN, 0);       // вырубить всё
//
//  }

  if (millis() - readTimer > 5000) {  // секундный таймер (для стабильности измерений)
    hum = dht.readHumidity();         // получить значение с датчика
     Serial.println(hum);
    readTimer = millis();             // обнулить таймер
  }
  delay(30);                         // задержка для стабильности
}
