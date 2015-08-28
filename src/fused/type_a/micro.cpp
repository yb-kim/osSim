#include <fused/type_a/micro.h>
#include <fused/type_a/microAppFactory.h>

MicroOSTypeA::MicroOSTypeA(Config *cfg) : MicroOS(cfg, false) {
    string osTypeString = "type_a";
    syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    factory = new TypeAMicroAppFactory();
    Syscall::setMonoSyscalls(syscallSpecs);
    Application::setApplications(appSpecs);
    return;
}
