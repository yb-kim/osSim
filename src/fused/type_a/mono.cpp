#include <fused/type_a/mono.h>
#include <fused/type_a/monoAppFactory.h>
#include <iostream>

using namespace std;

MonoOSTypeA::MonoOSTypeA(Config *cfg) : MonoOS(cfg) {
    delete factory;
    makeAppFactory();
    return;
}

void MonoOSTypeA::init() {
    OS::init();
    cout << "typeA init() called" << endl;
}

void MonoOSTypeA::checkAndDoSchedule() {
    for(unsigned int i=0; i<env->getNCores(); i++) {
        if(readyQueue->isEmpty()) return;
        Core *core = env->getCore(i);
        if(!core->isBusy()) {
            cout << "loading App to core " << core->getIndex() << endl;
            core->loadApp(readyQueue->deque());
        }
    }
}

void MonoOSTypeA::makeAppFactory() {
    factory = new TypeAMonoAppFactory(appWeights, nWeights);
}

void MonoOSTypeA::addAppToOS(int syscallIndex, int srcCoreIndex) {
    TypeAMonoAppFactory *factory = (TypeAMonoAppFactory *)factory;
    TypeAMonoApplication *syscallProcess = 
        factory->createSyscallApp(syscallIndex, srcCoreIndex);
    cout << "syscall process for syscall " << syscallIndex <<
        " has created" << endl;
    readyQueue->enque(syscallProcess);
    //
}


void MonoOSTypeA::processFinishedApp(int coreIndex) {
    unsigned int i = coreIndex;
    MonoCore* core = (MonoCore *)env->getCore(i);
    TypeAMonoApplication *app = (TypeAMonoApplication *)(core->getAppRunning());
    app->freeLock();
    app->setState(MonoApplication::NORMAL);
    FusedOSRequest *request = new FusedOSRequest(
            coreIndex, app->getTargetApplicationCoreIndex(), -1, false);
    cout << "dest core : #" << app->getTargetApplicationCoreIndex() << endl;
    fusedOS->sendIpc(request);
    delete app;
    core->freeCore();
    if(!readyQueue->isEmpty()) {
        cout << "load new app to core #" << coreIndex << endl;
        core->loadApp(readyQueue->deque());
    }
}
