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
    else osType = NONE;
}


OsConfig::OsConfig(Value& params) {
    maxTick = params["maxTick"].GetInt();
    unitTick = params["unitTick"].GetInt();
    contextSwitchTick = params["contextSwitchTick"].GetInt();
    nApps = params["nApps"].GetInt();
}


EnvConfig::EnvConfig(Value& params) {
    nCores = params["nCores"].GetInt();
    topology = params["topology"].GetString();
    baseLatency = params["baseLatency"].GetInt();
}


AppQueueConfig::AppQueueConfig(Value& params) {
    string policyString = params["policy"].GetString();
    Value& workloads = params["workloads"];
    nWorkloads = workloads.Size();
    workloadSequence = new int[nWorkloads];
    for(SizeType i=0; i<nWorkloads; i++) workloadSequence[i] = workloads[i].GetInt();
}
