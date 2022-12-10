#include <Arduino.h>
// #include <ESP8266WiFi.h>

// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include <WiFiManager.h>

#include "Lcd.h"
#include "Geiger.h"

Lcd* lcd = new Lcd();
Geiger* geiger = new Geiger();


void setup() {
  Serial.begin(9600);
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("GeigerCounterAP");
  // Serial.println("connected...yeey :)");

  lcd->setup();
  geiger->setup(13);
}

void loop() {
  geiger->loop();
  lcd->loop(geiger->getIsFullAccuracy(), geiger->getLastImpulses(), geiger->getCPM(), geiger->getUSv());
}