#include <fused/type_a/micro.h>
#include <fused/type_a/microAppFactory.h>

MicroOSTypeA::MicroOSTypeA(Config *cfg) : MicroOS(cfg, false) {
    string osTypeString = "type_a";
    syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    factory = new TypeAMicroAppFactory();
    TypeAMicroAppFactory *microAppFactory = (TypeAMicroAppFactory *)factory;
    microAppFactory->setOS(this);
    Syscall::setMonoSyscalls(syscallSpecs);
    Application::setApplications(appSpecs);
    return;
}

void MicroOSTypeA::sendIpc(FusedOSRequest *request) {
    fusedOS->sendIpc(request);
}


void MicroOSTypeA::resumeApplication(int coreIndex) {
    Core *core = env->getCore(coreIndex);
    TypeAMicroApplication *app = (TypeAMicroApplication *)(core->getAppRunning());
    app->setState(MicroApplication::COMPLETED);
}
