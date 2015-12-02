#pragma once

#include <syscall.h>
#include <micro/os.h>

class MicroSyscallSpec : public SyscallSpec {
public:
    MicroSyscallSpec(
            unsigned int _normalTicks,
            MicroOS::ServiceType *services,
            unsigned int nServices,
            std::string name,
            unsigned int _index);
    MicroSyscallSpec(int *workloads,
            unsigned int nWorkloads,
            std::string name,
            unsigned int index);
    //
    unsigned int getNServices() { return nWorkloads; return nServices; }
    std::string getName() { return name; }
    int getServiceType(unsigned int index) { return workloads[index]; }
    
private:
    unsigned int nServices;
    std::string name;
    MicroOS::ServiceType *services;
    int *workloads;
    unsigned int nWorkloads;
};
