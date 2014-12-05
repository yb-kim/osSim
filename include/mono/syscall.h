#pragma once

#include <syscall.h>
#include <string>

class MonoSyscallSpec : public SyscallSpec {
public:
    MonoSyscallSpec(unsigned int _normalTicks, unsigned int _lockTicks,
            std::string name):
        SyscallSpec(_normalTicks, _lockTicks),
        name(name)
    {
        //
    }

    std::string getName() { return name; }

private:
    std::string name;
};
