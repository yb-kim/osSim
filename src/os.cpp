#include <iostream>
#include <os.h> 
#include <cstdlib>
#include <ctime>

using namespace std;

OS::OS(Config *cfg) :
    osConfig(cfg->getOsConfig()),
    maxTick(osConfig->getMaxTick()),
    unitTick(osConfig->getUnitTick()),
    contextSwitchTick(osConfig->getContextSwitchTick()),
    nApps(osConfig->getNApps()),
    untilContextSwitch(0),
    curTick(0),
    nAppsFinished(0)
{
    readyQueue = new AppQueue(cfg->getAppQueueConfig());

    configFileRoot = cfg->getConfigFileRoot();
    osTypeString = cfg->getOsTypeString();

    syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    osSpecificSpecs = configFileRoot+osTypeString+"/system.json";
    //appSpecs = configFileRoot+osTypeString+"/apps.json";
    appSpecs = configFileRoot+"/apps.json";

    //Application::setApplications(appSpecs);

    srand(time(NULL));

}

void OS::run() {
    init();
    while(curTick <= maxTick) {
        checkAndDoSchedule();
        executeCores();
        increaseCurTick(unitTick);
        afterExecute();
    }
    wrapUp();
}

void OS::init() {
    for(unsigned int i=0; i<nApps; i++) {
        readyQueue->enque(factory->createApp());
    }
}

void OS::executeCore(unsigned int n, unsigned int unitTick) {
    Core *core = env->getCore(n);
    core->run(unitTick);
}


void OS::wrapUp() {
    cout << "total apps processed: " << nAppsFinished << endl;
    return;
}


void OS::executeCores() {
    cout << endl << endl << "<<Tick: " << curTick << ">>" << endl;
    for(int i=0; i<env->getNCores(); i++) {
        cout << "------------------" << endl;
        cout << "execute core " << env->getCore(i)->getPrintingIndex() << "..." << endl;
        executeCore(i, unitTick);
    }
}
