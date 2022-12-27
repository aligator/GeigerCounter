#include "Wifi.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "Config.h"

bool shouldSaveConfig = false;

void saveConfigCallback() {
  shouldSaveConfig = true;
}

Wifi::Wifi() {}

Wifi::~Wifi() {}
void Wifi::setup() {
  Config conf = ConfigLoader::load();

  char portStr[6]; 
  itoa(conf.mqttPort, portStr, 10);

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter customMqttHost("host", "mqtt host", conf.mqttHost, 64);
  WiFiManagerParameter customMqttPort("port", "mqtt port", portStr, 6);
  WiFiManagerParameter customMqttUser("user", "mqtt user", conf.mqttUser, 64);
  WiFiManagerParameter customMqttPassword("password", "mqtt password", conf.mqttPassword, 64);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&customMqttHost);
  wifiManager.addParameter(&customMqttPort);
  wifiManager.addParameter(&customMqttUser);
  wifiManager.addParameter(&customMqttPassword);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "GeigerCounter")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Config config = Config();
  strcpy(config.mqttHost, customMqttHost.getValue());
  config.mqttPort = atoi(customMqttPort.getValue());
  strcpy(config.mqttUser, customMqttUser.getValue());
  strcpy(config.mqttPassword, customMqttPassword.getValue());

  Serial.println(config.mqttHost);
  Serial.println(config.mqttPort);
  Serial.println(config.mqttUser);
  Serial.println(config.mqttPassword);

  if (shouldSaveConfig) {
    ConfigLoader::save(config);
  }
}

void Wifi::loop() {}

IPAddress Wifi::localIP() { return WiFi.localIP(); }
