#include <iostream>
#include <mono/os.h>
#include <mono/appFactory.h>
#include <mono/application.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {
    factory = new MonoAppFactory();

    Syscall::setSyscalls(syscallSpecs);
    MonoApplication::setApplications(appSpecs);
}

void MonoOS::init() {
    OS::init();
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
    cout << endl;
}
