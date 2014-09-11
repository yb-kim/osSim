#include <iostream>
#include <micro/os.h>
#include <micro/appFactory.h>

using namespace std;

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    factory = new MicroAppFactory();
    Syscall::setMicroSyscalls(syscallSpecs);
}

void MicroOS::init() {
    OS::init();
    //TODO: set core 0 as NS
    //TODO: set os service cores
}

void MicroOS::checkAndDoSchedule() {
    return;
}

void MicroOS::afterExecute() {
    return;
}
