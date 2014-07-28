#pragma onceo

#include <string>

class SyscallSpec;

class Syscall {
public:
    static void setSyscalls(std::string syscallSpecs);
    static SyscallSpec* getSyscallSpec(unsigned int n); 

private:
    Syscall() { }
    static SyscallSpec **syscalls;
};


class SyscallSpec {
public:
    SyscallSpec() { }
    SyscallSpec(unsigned int _normalTicks, unsigned int _lockTicks):
        normalTicks(_normalTicks), lockTicks(_lockTicks)
    {
        //
    }

    unsigned int getNormalTicks() { return normalTicks; }
    unsigned int getLockTicks() { return lockTicks; }

private:
    unsigned int normalTicks;
    unsigned int lockTicks;
};
