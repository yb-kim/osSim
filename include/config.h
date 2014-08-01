#pragma once

#include <lib/rapidjson/document.h>
#include <string>
#include <appQueue.h>

class OsConfig;
class EnvConfig;
class AppQueueConfig;

class Config {
public:
    Config(std::string configFile);

    enum OSType { NONE, MONO };

    OsConfig* getOsConfig() { return osConfig; }
    EnvConfig* getEnvConfig() { return envConfig; }
    OSType getOsType() { return osType; }
    std::string getOsTypeString() { return osTypeString; }

private:
    OsConfig *osConfig;
    EnvConfig *envConfig;
    AppQueueConfig *appQueueConfig;
    void setParams(rapidjson::Document& cfg);
    std::string osTypeString;
    OSType osType;
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



class AppQueueConfig {
public:
    AppQueueConfig(rapidjson::Value& params);
    AppQueue::SchedulePolicy getSchedulePolicy() { return schedulePolicy; }

private:
    AppQueue::SchedulePolicy schedulePolicy;
};
