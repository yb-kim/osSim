#include <iostream>
#include <os.h>

using namespace std;

OS::OS(Config *cfg) :
    osConfig(cfg->getOsConfig()),
    maxTick(osConfig->getMaxTick()),
    unitTick(osConfig->getUnitTick()),
    contextSwitchTick(osConfig->getContextSwitchTick()),
    untilContextSwitch(0),
    curTick(0)
{
    env = new Environment(cfg->getEnvConfig());
    readyQueue = new AppQueue();

    string osTypeString = cfg->getOsTypeString();
    string syscallSpecs = "config/"+osTypeString+"/syscalls.json";
    string appSpecs = "config/"+osTypeString+"/apps.json";

    Syscall::setSyscalls(syscallSpecs);
}

void OS::run() {
    init();
    while(curTick <= maxTick) {
        cout << "current Tick: " << curTick << endl;
        checkAndDoSchedule();
        for(int i=0; i<env->getNCores(); i++) {
            executeCore(i);
        }
        curTick += unitTick;
        afterExecute();
    }
    wrapUp();
}

void OS::init() {
    return;
}

void OS::executeCore(unsigned int n) {
    Core *core = env->getCore(n);
}

void OS::checkAndDoSchedule() {
    if(untilContextSwitch <= 0) {
        untilContextSwitch = contextSwitchTick;
    }
}

void OS::afterExecute() {
    untilContextSwitch -= unitTick;
}

void OS::wrapUp() {
    return;
}
