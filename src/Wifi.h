#ifndef WIFI_H_
#define WIFI_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Wifi {
public:
  Wifi();
  virtual ~Wifi();

  void setup();
  void loop();
  IPAddress localIP();

private:
};

#endif /* WIFI_H_ */
