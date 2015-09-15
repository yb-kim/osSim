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
    cout << "constructing MonoOS..." << endl;
    monoOS = new MonoOSTypeA(cfg);
    monoOS->setFusedOS(this);
    cfg->getEnvConfig()->setNCores(totalCores-nMonoCores);
    cfg->getEnvConfig()->setStartCoreIndex(nMonoCores);
    microOS = new MicroOSTypeA(cfg);
    cout << "constructing MicroOS..." << endl;
    microOS->setFusedOS(this);
    return;
}

void FusedOSTypeA::init() {
    //OS::init();
    cout << "run microOS->init()" << endl;
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

    remainingTicks = unitTick;
    processIpc(unitTick);
}


void FusedOSTypeA::sendIpc(FusedOSRequest *request) {
    cout << "(FusedOS): IPC request has recieved; sender: " << 
        request->srcCoreIndex << ", reciever: " <<
        request->destCoreIndex << ", syscllIdnex: " <<
        request->syscallIndex << endl;
    requestQueue.push(request);
}


void FusedOSTypeA::processIpc(int unitTick) {
    cout << "current queue size: " << requestQueue.size() << endl;
    if(requestQueue.empty()) return;
    do {
        FusedOSRequest* request = requestQueue.front();
        int srcCoreIndex = request->srcCoreIndex;
        int destCoreIndex = request->destCoreIndex;
        if(request->fromMicroOS) {
            srcCoreIndex = getGlobalCoreIndex(srcCoreIndex);
        }else {
            destCoreIndex = getGlobalCoreIndex(destCoreIndex);
        }
        int ipcCost = getIpcCost(srcCoreIndex, destCoreIndex);
        if(ipcCost <= remainingTicks) {
            requestQueue.pop();
            remainingTicks -= ipcCost;
            if(request->fromMicroOS) {
                monoOS->addAppToOS(request->syscallIndex,
                        request->srcCoreIndex);
            } else {
                microOS->resumeApplication(request->destCoreIndex);
            }
            cout << "remainigTicks: " << remainingTicks << endl;
        }
    } while(!requestQueue.empty() && remainingTicks > 0 && unitTick > 0);
}


int FusedOSTypeA::getIpcCost(int srcCoreIndex, int destCoreIndex) {
    return 5;
}
