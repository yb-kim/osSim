#include <micro/os.h>
#include <micro/appFactory.h>

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    factory = new MicroAppFactory();
}

void MicroOS::init() {
    OS::init();
}

void MicroOS::checkAndDoSchedule() {
    return;
}

void MicroOS::afterExecute() {
    return;
}
