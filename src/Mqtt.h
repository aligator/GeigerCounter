
#ifndef MQTT_H_
#define MQTT_H_

#include "Arduino.h"
#include "Config.h"
#include "GeigerStats.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class Mqtt {
public:
    Mqtt();
	virtual ~Mqtt();

	void setup(Config config);
    void loop(GeigerStats geigerStats);

private:
    Config config;

    WiFiClient espClient;
    PubSubClient client;

    GeigerStats lastGeigerStats;

    void _reconnect();
    void sendHADiscoveryMsg(DynamicJsonDocument doc, const char * sensor);
    
    static String _byte2str(byte* data, unsigned int length);
};

#endif /* MQTT_H_ */