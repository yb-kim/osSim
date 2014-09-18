#include <micro/application.h>

using namespace std;

MicroApplication::MicroApplication(unsigned int appSpecIndex) : Application(appSpecIndex) {
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

void MicroServiceApplication::run(unsigned int unitTick) {
    //
}
