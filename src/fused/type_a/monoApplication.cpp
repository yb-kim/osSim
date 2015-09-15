#include <fused/type_a/monoApplication.h>

TypeAMonoApplication::TypeAMonoApplication(unsigned int appSpecIndex,
        int targetApplicationCoreIndex) : 
    MonoApplication(appSpecIndex), 
    targetApplicationCoreIndex(targetApplicationCoreIndex)
{
    //
}

TypeAMonoApplication::TypeAMonoApplication(unsigned int appSpecIndex, 
        int syscallIndex, int targetApplicationCoreIndex) :
    MonoApplication(appSpecIndex), syscallIndex(syscallIndex), 
    targetApplicationCoreIndex(targetApplicationCoreIndex)
{
    SyscallSpec *syscall = Syscall::getSyscallSpec(syscallIndex);
}


void TypeAMonoApplication::processFinish() {
    MonoEnvironment *menv = (MonoEnvironment *)env;
    MonoOSTypeA *os = (MonoOSTypeA *)menv->getOS();
    os->processFinishedApp(coreIndex);
}
