#pragma once

#include <syscall.h>

class MonoSyscallSpec : public SyscallSpec {
public:
    MonoSyscallSpec(unsigned int _normalTicks, unsigned int _lockTicks):
        SyscallSpec(_normalTicks, _lockTicks)
    {
        //
    }

private:
    //
};
