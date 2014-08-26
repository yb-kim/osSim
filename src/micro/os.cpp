#include <micro/os.h>

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    //
}

void MicroOS::init() {
    OS::init();
}
