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
    state = MicroApplication::WAITING;
    MicroOSTypeA* microOS = (MicroOSTypeA *)os;
    FusedOSRequest *request = new FusedOSRequest(
            this->getCoreIndex(), 0, syscallSpec->getIndex(), true);
    microOS->sendIpc(request);
    return;
}


bool TypeAMicroApplication::isSyscallFinished() {
    return (pc.spec == NULL && state == NORMAL && pc.normalTicks <= 0) || (state == COMPLETED);
}
