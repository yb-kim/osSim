#include <iostream>
#include <monoOS.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {
    string syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    string appSpecs = configFileRoot+osTypeString+"/apps.json";
    Syscall::setSyscalls(syscallSpecs);
    Application::setApplications(appSpecs);
}

void MonoOS::init() {
    OS::init();
}
