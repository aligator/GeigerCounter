#include "Config.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

ConfigLoader::ConfigLoader() {}

void ConfigLoader::reset()
{
  LittleFS.format();
}

Config ConfigLoader::load()
{
  Config config = Config{};

  // read configuration from FS json
  Serial.println("mounting FS...");

  if (!LittleFS.begin())
  {
    Serial.println("failed to mount FS");
    return config;
  }

  Serial.println("mounted file system");
  if (!LittleFS.exists(CONFIG_FILE))
  {
    Serial.println("file does not exist");
    LittleFS.end();
    return config;
  }

  // file exists, reading and loading
  Serial.println("reading config file");
  File configFile = LittleFS.open(CONFIG_FILE, "r");
  if (!configFile)
  {
    Serial.println("reading the file failed");
    LittleFS.end();
    return config;
  }

  Serial.println("opened config file");
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  DynamicJsonDocument json(1024);
  auto deserializeError = deserializeJson(json, buf.get());
  serializeJson(json, Serial);
  if (!deserializeError)
  {
    Serial.println("\nparsed json");
    strcpy(config.mqttHost, json["mqttHost"]);
    config.mqttPort = json["mqttPort"].as<uint>();
    config.doReset = json["doReset"].as<bool>();
    strcpy(config.mqttUser, json["mqttUser"]);
    strcpy(config.mqttPassword, json["mqttPassword"]);
  }
  else
  {
    Serial.println("failed to load json config");
  }
  configFile.close();

  LittleFS.end();

  return config;
}

void ConfigLoader::save(Config newConfig)
{
  // save the custom parameters to FS
  Serial.println("saving config");
  DynamicJsonDocument json(1024);
  json["mqttHost"] = newConfig.mqttHost;
  json["mqttPort"] = newConfig.mqttPort;
  json["mqttUser"] = newConfig.mqttUser;
  json["mqttPassword"] = newConfig.mqttPassword;
  json["doReset"] = newConfig.doReset;

  Serial.println("mounting FS...");

  if (!LittleFS.begin())
  {
    Serial.println("failed to mount FS");
    return;
  }

  Serial.println("mounted file system");

  File configFile = LittleFS.open(CONFIG_FILE, "w");
  if (!configFile)
  {
    Serial.println("failed to open config file for writing");
  }


  serializeJson(json, Serial);
  if(serializeJson(json, configFile)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }

  configFile.close();

  LittleFS.end();
}
