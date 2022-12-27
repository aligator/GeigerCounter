#ifndef WIFI_H_
#define WIFI_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#define WEBSERVER_H // needed to work with the wifimanager
#include "ESPAsyncWebServer.h"

class Wifi {
public:
  Wifi();
  virtual ~Wifi();

  void setup();
  void loop();
  IPAddress localIP();

private:
  AsyncWebServer server = AsyncWebServer(80);
};

#endif /* WIFI_H_ */
