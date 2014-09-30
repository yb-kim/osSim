#pragma once

#include <syscall.h>
#include <micro/os.h>

class MicroSyscallSpec : public SyscallSpec {
public:
    MicroSyscallSpec(unsigned int _normalTicks,
            MicroOS::ServiceType *services,
            unsigned int nServices);
    //
    unsigned int getNServices() { return nServices; }
    MicroOS::ServiceType getServiceType(unsigned int index) { return services[index]; }
    
private:
    unsigned int nServices;
    MicroOS::ServiceType *services;
};
