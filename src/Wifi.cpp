#include "Wifi.h"
#include "Config.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Geiger Counter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
  </style>
</head>
<body>
  <h2>Geiger Counter</h2>
  <a href="/reset">Reset all settings</a>
</body>
</html>
)rawliteral";


bool shouldSaveConfig = false;
void saveConfigCallback() {
  shouldSaveConfig = true;
}

bool reset = false;

Wifi::Wifi() {}

Wifi::~Wifi() {}

Config Wifi::setup() {
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
  bool ok = false;
  Config newConfig = Config();
  if (conf.doReset) {
    ok = wifiManager.startConfigPortal("AutoConnectAP", "GeigerCounter");
  } else {
    ok = wifiManager.autoConnect("AutoConnectAP", "GeigerCounter");
    // Preserve the clientId only if it was no reset.
    newConfig.clientId = conf.clientId;
  }

  if (!ok) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  
  strcpy(newConfig.mqttHost, customMqttHost.getValue());
  newConfig.mqttPort = atoi(customMqttPort.getValue());
  strcpy(newConfig.mqttUser, customMqttUser.getValue());
  strcpy(newConfig.mqttPassword, customMqttPassword.getValue());

  if (shouldSaveConfig) {
    ConfigLoader::save(newConfig);
  }

  this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  this->server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    reset = true;
    request->send(200, "text/plain", "OK");
  });

  this->server.begin();

  return newConfig;
}

void Wifi::loop() {
  if (reset) {
    Config config = ConfigLoader::load();
    config.doReset = true;
    ConfigLoader::save(config);
    ESP.restart();
    delay(3000);
  }
}

IPAddress Wifi::localIP() { return WiFi.localIP(); }
