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
    Syscall::setMonoSyscalls(syscallSpecs);
    env = new MonoEnvironment(cfg->getEnvConfig(), this);
    factory = new MonoAppFactory();
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
            Core* core = env->getCore(i);
            MonoApplication *app = (MonoApplication *)(core->getAppRunning());
            app->freeLock();
            if(app->isFinished()) {
                nAppsFinished++;
                delete app;
                readyQueue->enque(factory->createApp());
            } else {
                readyQueue->enque(app);
            }
            core->loadApp(readyQueue->deque());
        }
    }
}

void MonoOS::afterExecute() {
    untilContextSwitch -= unitTick;
    MonoEnvironment *menv = (MonoEnvironment *)env;
    unsigned int nRequests = unitTick / coherencyRequestTicks;
    cout << "------------------" << endl;
    cout << "start processing coherency requests" << endl;
    cout << "maximum of " << nRequests << " requests can be processed" << endl;
    cout << menv->getBus()->getQueueSize() << " requests in the queue" << endl;
    for(unsigned int i=0; i<nRequests; i++) {
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
            Core *core = env->getCore(i);
            MonoApplication *app = (MonoApplication *)(core->getAppRunning());
            processed = app->snoopBus(req);
            if(processed) break;
        }

        if(processed) continue;
        
        //If no core can process request (must be handled by memory)
        unsigned int syscallIndex = req->syscallIndex;
        switch(req->requestType) {
        case CoherencyRequest::READ: 
            cout << "request is processed by memory "
                << "and changed state to EXCLUSIVE" << endl;
            req->src->setCacheState(syscallIndex, MonoCore::EXCLUSIVE);
            break;

        case CoherencyRequest::INVALIDATION:
            req->src->setCacheState(syscallIndex, MonoCore::MODIFIED);
            break;
        }
        MonoApplication *app = (MonoApplication *)req->src->getAppRunning();
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
}
