#include <fused/type_a/mono.h>
#include <iostream>

using namespace std;

MonoOSTypeA::MonoOSTypeA(Config *cfg) : MonoOS(cfg) {
    return;
}

void MonoOSTypeA::init() {
    OS::init();
    cout << "typeA init() called" << endl;
}

void MonoOSTypeA::checkAndDoSchedule() {
    return;
}
