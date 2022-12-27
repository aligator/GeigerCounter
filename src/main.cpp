#include <Arduino.h>

#include "Lcd.h"
#include "Geiger.h"
#include "Wifi.h"
#include "Config.h"

Wifi* wifi = new Wifi();
Lcd* lcd = new Lcd();
Geiger* geiger = new Geiger();


void setup() {
  Serial.begin(9600);
  
  wifi->setup();
  lcd->setup();
  geiger->setup(13);

  const Config config = ConfigLoader::load();
}

void loop() {
  wifi->loop();
  geiger->loop();
  lcd->loop(geiger->getStats(), wifi->localIP().toString());
}