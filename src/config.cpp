#include <config.h>
#include <fstream>
#include <iostream>
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
}


OsConfig::OsConfig(Value& params) {
    maxTick = params["maxTick"].GetInt();
    unitTick = params["unitTick"].GetInt();
    contextSwitchTick = params["contextSwitchTick"].GetInt();
}


EnvConfig::EnvConfig(Value& params) {
    nCores = params["nCores"].GetInt();
}
