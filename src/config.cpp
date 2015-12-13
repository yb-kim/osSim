#include <config.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace rapidjson;

Config::Config(string configFileRoot):
    configFileRoot(configFileRoot)
{
    //read config file and set params
    string configFile = configFileRoot + "/system.json";
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
    else if(osTypeString == "micro") osType = MICRO;
    else if(osTypeString == "type_a") osType = TYPE_A;
    else osType = NONE;
}


OsConfig::OsConfig(Value& params) {
    maxTick = params["maxTick"].GetInt();
    unitTick = params["unitTick"].GetInt();
    contextSwitchTick = params["contextSwitchTick"].GetInt();
    nApps = params["nApps"].GetInt();
    nWeights = params["appWeights"].Size();
    weights = new int[nWeights];
    for(int i=0; i<nWeights; i++) {
        weights[i] = params["appWeights"][i].GetInt();
    }
}


EnvConfig::EnvConfig(Value& params) {
    nCores = params["nCores"].GetInt();
    topology = params["topology"].GetString();
    baseLatency = params["baseLatency"].GetInt();
    ipc_die = params["ipc_die"].GetInt();
    ipc_remote = params["ipc_remote"].GetInt();
    startCoreIndex = 0;
}


AppQueueConfig::AppQueueConfig(Value& params) {
    string policyString = params["policy"].GetString();
}
