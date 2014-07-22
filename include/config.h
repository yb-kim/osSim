#include <lib/rapidjson/document.h>
#include <string>

class Config {
public:
    Config(std::string configFile);
    unsigned int getMaxTick() { return maxTick; }
    unsigned int getUnitTick() { return unitTick; }
    unsigned int getNCores() { return nCores; }

private:
    void setParams(rapidjson::Document &cfg);
    unsigned int maxTick;
    unsigned int unitTick;
    unsigned int nCores;
};
