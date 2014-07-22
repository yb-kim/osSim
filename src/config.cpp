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

void Config::setParams(Document &cfg) {
    maxTick = cfg["maxTick"].GetInt();
    unitTick = cfg["unitTick"].GetInt();
    nCores = cfg["nCores"].GetInt();
}
