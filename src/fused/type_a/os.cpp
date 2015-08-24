#include <fused/type_a/os.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <lib/rapidjson/document.h>

using namespace std;
using namespace rapidjson;

FusedOSTypeA::FusedOSTypeA(Config *cfg) : OS(cfg) {
    setOsSpecificSpecs(osSpecificSpecs);
    unsigned int totalCores = cfg->getEnvConfig()->getNCores();
    cfg->getEnvConfig()->setNCores(nMonoCores);
    monoOS = new MonoOSTypeA(cfg);
    cfg->getEnvConfig()->setNCores(totalCores-nMonoCores);
    cfg->getEnvConfig()->setStartCoreIndex(nMonoCores);
    microOS = new MicroOSTypeA(cfg);
    return;
}

void FusedOSTypeA::init() {
    //OS::init();
    microOS->init();
}

void FusedOSTypeA::checkAndDoSchedule() {
    return;
}

void FusedOSTypeA::afterExecute() {
    return;
}

void FusedOSTypeA::setOsSpecificSpecs(string osSpecificSpecs) {
    ifstream inFile(osSpecificSpecs.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());
    Document specConfig;
    specConfig.Parse(jsonConfig.c_str());

    nMonoCores = specConfig["nMonoCores"].GetInt();
    return;
}

void FusedOSTypeA::executeCores() {
    cout << endl << endl << "<<Tick: " << curTick << ">>" << endl;
    cout << "running mono OS part" << endl;
    monoOS->checkAndDoSchedule();
    monoOS->executeCores();
    monoOS->increaseCurTick(unitTick);
    monoOS->afterExecute();

    cout << "running micro OS part" << endl;
    microOS->checkAndDoSchedule();
    microOS->executeCores();
    microOS->increaseCurTick(unitTick);
    microOS->afterExecute();
}
