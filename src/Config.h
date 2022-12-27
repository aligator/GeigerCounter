#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

#define CONFIG_FILE "/config.json"

struct Config {
  char mqttHost[64];
  uint mqttPort;
  char mqttUser[64];
  char mqttPassword[64];
};

class ConfigLoader {
public:
  static void reset();
  static Config load();
  static void save(Config newConfig);
private:
  ConfigLoader();
};

#endif /* CONFIG_H_ */

