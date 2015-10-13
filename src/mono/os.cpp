#include <iostream>
#include <mono/os.h>
#include <mono/appFactory.h>
#include <mono/environment.h>
#include <mono/application.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace rapidjson;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {
    string osTypeString = "mono";
    syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    osSpecificSpecs = configFileRoot+osTypeString+"/system.json";
    appSpecs = configFileRoot+"/apps.json";

    Syscall::setMonoSyscalls(syscallSpecs);
    Application::setApplications(appSpecs);
    env = new MonoEnvironment(cfg->getEnvConfig(), this);
    makeAppFactory();
    setOsSpecificSpecs(osSpecificSpecs);
}

void MonoOS::init() {
    OS::init();
    for(unsigned int i=0; i<env->getNCores(); i++) {
        env->getCore(i)->loadApp(readyQueue->deque());
    }
}

void MonoOS::checkAndDoSchedule() {
    if(untilContextSwitch <= 0) {
        cout << "context switching..." << endl;
        untilContextSwitch = contextSwitchTick;
        //context switch
        for(unsigned int i=0; i<env->getNCores(); i++) {
            switchApp(i);
        }
        //flush bus
        MonoEnvironment *menv = (MonoEnvironment *)env;
        menv->getBus()->flushQueue();
    }
}

void MonoOS::afterExecute() {
    untilContextSwitch -= unitTick;
    MonoEnvironment *menv = (MonoEnvironment *)env;
    int remainingTicks = unitTick;
    //unsigned int nRequests = unitTick / coherencyRequestTicks;
    cout << "------------------" << endl;
    cout << "start processing coherency requests" << endl;
    //cout << "maximum of " << nRequests << " requests can be processed" << endl;
    cout << menv->getBus()->getQueueSize() << " requests in the queue" << endl;
    while(remainingTicks >= coherencyRequestTicksInDie) {
        CoherencyRequest *req = menv->getBus()->deque();
        if(!req) {
            cout << "no request to process" << endl;
            break;
        }
        //If there is request to process
        bool processed = false;
        cout << "processing request: " << req->getDescription() << endl;
        for(unsigned int i=0; i<env->getNCores(); i++) {
            MonoCore *srcCore = req->src;
            Core *targetCore = env->getCore(i);
            if(targetCore->isBusy()) {
                MonoApplication *app = (MonoApplication *)(targetCore->getAppRunning());
                processed = app->snoopBus(req);
                if(processed) {
                    int cost = getCoherencyCost(req->src->getIndex(), i);
                    remainingTicks -= cost;
                    cout << "coherency Request consumes " << cost << " ticks" << endl;
                    break;
                }
            }
        }

        //If the request is successfully snooped, try process next request
        if(processed) continue;
        
        //If no core can process request 
        //(must be handled by memory or the request is invalidation)
        unsigned int syscallIndex = req->syscallIndex;
        int cost;
        MonoApplication *app = (MonoApplication *)req->src->getAppRunning();
        switch(req->requestType) {
        case CoherencyRequest::READ: 
            cout << "request is processed by memory "
                << "and changed state to EXCLUSIVE" << endl;
            req->src->setCacheState(syscallIndex, MonoCore::EXCLUSIVE);
            cost = getCoherencyCost(req->src->getIndex(), -1);
            remainingTicks -= cost;
            cout << "coherency Request consumes " << cost << " ticks" << endl;
            break;

        case CoherencyRequest::INVALIDATION:
            req->src->setCacheState(syscallIndex, MonoCore::MODIFIED);
            Syscall::getLock(syscallIndex, app);
            cost = getCoherencyCost(req->src->getIndex(), -2);
            remainingTicks -= cost;
            cout << "coherency Request consumes " << cost << " ticks" << endl;
            break;
        }
        app->setState(MonoApplication::NORMAL);

    }
    cout << endl;
}


void MonoOS::setOsSpecificSpecs(std::string osSpecificSpecs) {
    ifstream inFile(osSpecificSpecs.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());
    Document specConfig;
    specConfig.Parse(jsonConfig.c_str());

    coherencyRequestTicks = specConfig["coherencyRequestTicks"].GetInt();
    coherencyRequestTicksInDie = specConfig["coherencyRequestTicksInDie"].GetInt();
    coherencyRequestTicksOneHop = specConfig["coherencyRequestTicksOneHop"].GetInt();
    coherencyRequestTicksTwoHops = specConfig["coherencyRequestTicksTwoHops"].GetInt();
    oneHopRatio = specConfig["oneHopRatio"].GetDouble();
    twoHopsRatio = specConfig["twoHopsRatio"].GetDouble();
    inDieRatio = 1 - oneHopRatio - twoHopsRatio;
}


void MonoOS::switchApp(unsigned int coreIndex) {
    unsigned int i = coreIndex;
    MonoCore* core = (MonoCore *)env->getCore(i);
    MonoApplication *app = (MonoApplication *)(core->getAppRunning());
    app->freeLock();
    app->setState(MonoApplication::NORMAL);
    if(app->isFinished()) {
        nAppsFinished++;
        delete app;
        readyQueue->enque(factory->createApp());
    } else {
        readyQueue->enque(app);
    }
    core->loadApp(readyQueue->deque());
}


int MonoOS::getCoherencyCost(int requestSrc, int dest) {
    if(dest == -1) { //from memory
        //
    }

    if(dest == -2) { //invalidation
        //
    }
    return (env->getMessageCost(requestSrc, dest));
}


void MonoOS::makeAppFactory() {
    factory = new MonoAppFactory(appWeights, nWeights);
}
