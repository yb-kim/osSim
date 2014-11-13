#pragma once

#include <syscall.h>
#include <micro/os.h>

class MicroSyscallSpec : public SyscallSpec {
public:
    MicroSyscallSpec(unsigned int _normalTicks,
            MicroOS::ServiceType *services,
            unsigned int nServices,
            std::string name);
    //
    unsigned int getNServices() { return nServices; }
    std::string getName() { return name; }
    MicroOS::ServiceType getServiceType(unsigned int index) { return services[index]; }
    
private:
    unsigned int nServices;
    std::string name;
    MicroOS::ServiceType *services;
};
