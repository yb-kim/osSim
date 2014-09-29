#include <micro/application.h>
#include <iostream>

using namespace std;

MicroApplication::MicroApplication(int appSpecIndex) : Application(appSpecIndex) {
    //
}

void MicroApplication::run(unsigned int unitTick) {
    //
}

void MicroApplication::ipc(MicroOS::ServiceType serviceType) {
    //
}



MicroServiceApplication::MicroServiceApplication() : MicroApplication(-1) {
    //
}

MicroServiceApplication::MicroServiceApplication(
        MicroOS::ServiceType service):
    MicroApplication(-1)
{
    //
}

void MicroServiceApplication::run(unsigned int unitTick) {
    //check queue
    if(requestQueue.empty()) {
        return;
    }
}

NSServiceApplication::NSServiceApplication() : MicroServiceApplication() {
    //
}

void NSServiceApplication::run(unsigned int unitTick) {
    //
}
