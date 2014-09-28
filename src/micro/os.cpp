#include <iostream>
#include <micro/os.h>
#include <micro/appFactory.h>
#include <micro/application.h>

using namespace std;

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    factory = new MicroAppFactory();
    Syscall::setMicroSyscalls(syscallSpecs);
}

void MicroOS::init() {
    //OS::init();
    //TODO: set core 0 as NS
    env->getCore(0)->loadApp(new NSServiceApplication());
    //TODO: set os service cores
    for(int i=FM; i<SERVICES_END; i++) {
        env->getCore(i)->loadApp(
                new MicroServiceApplication((Services)i));
    }
}

void MicroOS::checkAndDoSchedule() {
    return;
}

void MicroOS::afterExecute() {
    return;
}
