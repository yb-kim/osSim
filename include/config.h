#pragma once

#include <lib/rapidjson/document.h>
#include <string>

class OsConfig;
class EnvConfig;

class Config {
public:
    Config(std::string configFile);
    OsConfig* getOsConfig() { return osConfig; }
    EnvConfig* getEnvConfig() { return envConfig; }

private:
    OsConfig *osConfig;
    EnvConfig *envConfig;
    void setParams(rapidjson::Document& cfg);
};


class OsConfig {
public:
    OsConfig(rapidjson::Value& params);
    unsigned int getMaxTick() { return maxTick; }
    unsigned int getUnitTick() { return unitTick; }
    unsigned int getNCores() { return nCores; }
    unsigned int getContextSwitchTick() { return contextSwitchTick; }

private:
    unsigned int maxTick;
    unsigned int unitTick;
    unsigned int nCores;
    unsigned int contextSwitchTick;
};


class EnvConfig {
public:
    EnvConfig(rapidjson::Value& params);
    unsigned int getNCores() { return nCores; }

private:
    unsigned int nCores;
};
