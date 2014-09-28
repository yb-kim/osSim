#include <micro/application.h>
#include <iostream>

using namespace std;

MicroApplication::MicroApplication(int appSpecIndex) : Application(appSpecIndex) {
    //
}

void MicroApplication::run(unsigned int unitTick) {
    //
}

void MicroApplication::ipc(MicroOS::Services service) {
    //
}



MicroServiceApplication::MicroServiceApplication() : MicroApplication(-1) {
    //
}

MicroServiceApplication::MicroServiceApplication(
        MicroOS::Services service):
    MicroApplication(-1),
    service(service)
{
    //
}

void MicroServiceApplication::run(unsigned int unitTick) {
    cout << service << endl;
}

NSServiceApplication::NSServiceApplication() : MicroServiceApplication() {
    //
}

void NSServiceApplication::run(unsigned int unitTick) {
    //
}
