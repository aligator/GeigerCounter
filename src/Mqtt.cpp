#include "Mqtt.h"

#define MQTT_TOPIC "/geiger"
#define TOPIC_TICK_15S MQTT_TOPIC "/last15s"
#define TOPIC_TICK_60S MQTT_TOPIC "/last60s"
#define TOPIC_USV MQTT_TOPIC "/uSv"

/**
 * converts bytes to a String
 */
String Mqtt::_byte2str(byte* data, unsigned int length) {
    char* buffer = (char*) malloc (length+1);
    memcpy(buffer, data, length);
    buffer[length] = 0;

    // or for unicode:
    //const char* str((const wchar_t*)buffer);
    String out = String((const char*) buffer);
    free(buffer);
    return out;
}

Mqtt::Mqtt() {
    this->client = PubSubClient(this->espClient);
}

Mqtt::~Mqtt() {}

void Mqtt::setup(Config config) {
    this->config = config;
    delay(10);
    this->client.setServer(this->config.mqttHost, this->config.mqttPort);
}

void Mqtt::_reconnect() {
    if (!client.connected()) {
        Serial.print("Reconnecting...");

        String clientId = "Geiger-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (!client.connect(clientId.c_str(), this->config.mqttUser, this->config.mqttPassword)) {
            Serial.print("failed, rc=");
            Serial.println(client.state());
        } else {
            Serial.println("Connected");

            Serial.println("Sending HA discovery.");
            DynamicJsonDocument doc(1024);
            doc["name"] = "Geiger Counter Ticks 15s";
            doc["stat_t"] = TOPIC_TICK_15S;
            doc["frc_upd"] = true;
            doc["icon"] = "mdi:clock-time-three";
            this->sendHADiscoveryMsg(doc, TOPIC_TICK_15S);
            doc.clear();

            doc["name"] = "Geiger Counter Ticks 60s";
            doc["stat_t"] = TOPIC_TICK_60S;
            doc["frc_upd"] = true;
            doc["icon"] = "mdi:clock-time-twelve";
            this->sendHADiscoveryMsg(doc, TOPIC_TICK_60S);
            doc.clear();

            doc["name"] = "Geiger Counter µSv";
            doc["stat_t"] = TOPIC_USV;
            doc["unit_of_meas"] = "µSv/h";
            doc["frc_upd"] = true;
            doc["icon"] = "mdi:atom-variant";
            this->sendHADiscoveryMsg(doc, TOPIC_USV);

            Serial.println("DONE!");
        }
    }
}

void Mqtt::sendHADiscoveryMsg(DynamicJsonDocument doc, const char * sensor) {
  String discoveryTopic = String("homeassistant/sensor");
  discoveryTopic.concat(sensor);
  discoveryTopic.concat("/config");

  char buffer[256];

  size_t n = serializeJson(doc, buffer);
  this->client.publish(discoveryTopic.c_str(), buffer, n);
}

void Mqtt::loop(GeigerStats geigerStats) {
    if (!client.connected()) {
        _reconnect();
    } else {
        client.loop();

        if (!geigerStats.isFullAccuracy) {
            return;
        }

        if (geigerStats.lastImpulses != this->lastGeigerStats.lastImpulses ||
            geigerStats.cpm != this->lastGeigerStats.cpm ||
            geigerStats.uSv != this->lastGeigerStats.uSv
        ) {
            this->lastGeigerStats = geigerStats;
        
            char lastImpulses[10];
            char cpm[10];
            char uSv[10];
            itoa(geigerStats.lastImpulses, lastImpulses, 10);
            itoa(geigerStats.cpm, cpm, 10);
            dtostrf(geigerStats.uSv, 0, 4, uSv);
            this->client.publish(TOPIC_TICK_15S, lastImpulses);
            this->client.publish(TOPIC_TICK_60S, cpm);
            this->client.publish(TOPIC_USV, uSv);
        }
    }
}