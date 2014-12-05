#include <iostream>
#include <mono/os.h>
#include <mono/appFactory.h>
#include <mono/environment.h>
#include <mono/application.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {
    Syscall::setMonoSyscalls(syscallSpecs);
    env = new MonoEnvironment(cfg->getEnvConfig(), this);
    factory = new MonoAppFactory();
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
    for(unsigned int i=0; i<3; i++) {
        CoherencyRequest *req = menv->getBus()->deque();
        if(!req) {
            cout << "no request to process" << endl;
            break;
        }
        //If there is request to process
        bool processed = false;
        for(unsigned int i=0; i<env->getNCores(); i++) {
            MonoCore *srcCore = req->src;
            cout << "process request from core " << srcCore->getIndex() << endl;
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

