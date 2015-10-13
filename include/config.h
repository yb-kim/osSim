#pragma once

#include <lib/rapidjson/document.h>
#include <string>

class OsConfig;
class EnvConfig;
class AppQueueConfig;

class Config {
public:
    Config(std::string configFile);

    enum OSType { NONE, MONO, MICRO, TYPE_A };

    std::string getConfigFileRoot() { return configFileRoot; }
    OsConfig* getOsConfig() { return osConfig; }
    EnvConfig* getEnvConfig() { return envConfig; }
    AppQueueConfig* getAppQueueConfig() { return appQueueConfig; }
    OSType getOsType() { return osType; }
    std::string getOsTypeString() { return osTypeString; }

private:
    std::string configFileRoot;
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
    unsigned int getNApps() { return nApps; }
    int getNWeights() { return nWeights; }
    int* getAppWeights() { return weights; }


private:
    unsigned int maxTick;
    unsigned int unitTick;
    unsigned int nCores;
    unsigned int contextSwitchTick;
    unsigned int nApps;
    int nWeights;
    int *weights;
};


class EnvConfig {
public:
    EnvConfig(rapidjson::Value& params);
    unsigned int getNCores() { return nCores; }
    std::string getTopology() { return topology; }
    unsigned int getBaseLatency() { return baseLatency; }
    unsigned int getStartCoreIndex() { return startCoreIndex; }

    void setStartCoreIndex(unsigned int i) { startCoreIndex = i; }

    void setNCores(int n) { nCores = n; }

private:
    unsigned int nCores;
    unsigned int baseLatency;
    std::string topology;
    unsigned int startCoreIndex;
};



class AppQueueConfig {
public:
    AppQueueConfig(rapidjson::Value& params);
    std::string getSchedulePolicyString() { return schedulePolicyString; }
    int getNWorkloads() { return nWorkloads; }
    int* getWorkloadSequence() { return workloadSequence; }

private:
    std::string schedulePolicyString;
    int nWorkloads;
    int *workloadSequence;
};
