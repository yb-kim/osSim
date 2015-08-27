#include <fused/type_a/microApplication.h>
#include <iostream>

using namespace std;

TypeAMicroApplication::TypeAMicroApplication(
        unsigned int appSpecIndex) :
    MicroApplication(appSpecIndex) 
{
    //
}


void TypeAMicroApplication::doIpc() {
    int *syscallIndex = spec->getSyscallIndex();
    MicroSyscallSpec *syscallSpec = (MicroSyscallSpec *)
        Syscall::getSyscallSpec(syscallIndex[syscallPointer]);
    cout << "current syscall: " << syscallSpec->getName() << endl;
    return;
}
