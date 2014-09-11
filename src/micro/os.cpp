#include <iostream>
#include <micro/os.h>
#include <micro/appFactory.h>

using namespace std;

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    cout << "MicroOS created" << endl;
    factory = new MicroAppFactory();
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
