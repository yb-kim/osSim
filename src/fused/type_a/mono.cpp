#include <fused/type_a/mono.h>
#include <fused/type_a/monoAppFactory.h>
#include <iostream>

using namespace std;

MonoOSTypeA::MonoOSTypeA(Config *cfg) : MonoOS(cfg) {
    delete factory;
    makeAppFactory();
    return;
}

void MonoOSTypeA::init() {
    OS::init();
    cout << "typeA init() called" << endl;
}

void MonoOSTypeA::checkAndDoSchedule() {
    return;
}

void MonoOSTypeA::makeAppFactory() {
    factory = new TypeAMonoAppFactory();
}
