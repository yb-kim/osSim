#include <config.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace rapidjson;

Config::Config(string configFile) {
    //read config file and set params
    ifstream inFile(configFile.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document cfg;
    cfg.Parse(jsonConfig.c_str());
    setParams(cfg);
}


void Config::setParams(Document& cfg) {
    osConfig = new OsConfig(cfg["os"]);
    envConfig = new EnvConfig(cfg["env"]);
    appQueueConfig = new AppQueueConfig(cfg["appQueue"]);

    osTypeString = cfg["osType"].GetString();
    if(osTypeString == "mono") osType = MONO;
    else osType = NONE;
}


OsConfig::OsConfig(Value& params) {
    maxTick = params["maxTick"].GetInt();
    unitTick = params["unitTick"].GetInt();
    contextSwitchTick = params["contextSwitchTick"].GetInt();
}


EnvConfig::EnvConfig(Value& params) {
    nCores = params["nCores"].GetInt();
}


AppQueueConfig::AppQueueConfig(Value& params) {
    string policyString = params["policy"].GetString();
    if(policyString == "ROUND_ROBIN") schedulePolicy = AppQueue::ROUND_ROBIN;
    else schedulePolicy = AppQueue::ROUND_ROBIN; //default
}
