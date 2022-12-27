#include <Arduino.h>

#include "Config.h"
#include "Lcd.h"
#include "Geiger.h"
#include "Wifi.h"
#include "Mqtt.h"

Wifi* wifi = new Wifi();
Lcd* lcd = new Lcd();
Geiger* geiger = new Geiger();
Mqtt* mqtt = new Mqtt();


void setup() {
  Serial.begin(9600);
  
  Config config = wifi->setup();
  lcd->setup();
  geiger->setup(13);

  mqtt->setup(config);
}

void loop() {
  wifi->loop();
  geiger->loop();
  lcd->loop(geiger->getStats(), wifi->localIP().toString());
  mqtt->loop(geiger->getStats());
}