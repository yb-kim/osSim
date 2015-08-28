#pragma once

#include <syscall.h>
#include <string>

class MonoSyscallSpec : public SyscallSpec {
public:

    enum WorkloadType {
        NORMAL,
        LOCK
    };

    typedef struct syscallWorkload {
        WorkloadType type;
        unsigned int tick;
    } SyscallWorkload;

    MonoSyscallSpec(unsigned int _normalTicks, unsigned int _lockTicks,
            std::string name, unsigned int index):
        SyscallSpec(_normalTicks, _lockTicks, index),
        name(name)
    {
        //
    }

    MonoSyscallSpec(SyscallWorkload *syscallWorkloads,
            std::string name,
            unsigned int index) :
        syscallWorkloads(syscallWorkloads), name(name), SyscallSpec(0, 0, index)
    {
        //
    }

    std::string getName() { return name; }


private:
    std::string name;
    SyscallWorkload *syscallWorkloads;
};
