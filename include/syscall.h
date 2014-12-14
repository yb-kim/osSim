#pragma once

#include <string>
#include <application.h>
#include <lib/rapidjson/document.h>

class SyscallSpec;
class Application;

class Syscall {
public:
    //static void setSyscalls(std::string syscallSpecs);
    static SyscallSpec* getSyscallSpec(unsigned int n); 
    static bool getLock(unsigned int syscallNum, Application *app);
    static bool checkLock(unsigned int syscallNum, Application *app);
    static void freeLock(unsigned int syscallNum, Application *app);
    static bool isTakingLock(unsigned int syscallNum, Application *app);

    static void setMonoSyscalls(std::string syscallSpecs);
    static void setMicroSyscalls(std::string syscallSpecs);
    static unsigned int getNSpecs() { return nSpecs; }

private:
    Syscall() { }
    static void readConfig(std::string syscallSpecs);
    static SyscallSpec **syscalls;
    static Application **locks;
    static rapidjson::Document* specConfig;
    static unsigned int nSpecs;
};


class SyscallSpec {
public:
    SyscallSpec() { }
    SyscallSpec(unsigned int _normalTicks, unsigned int _lockTicks,
            unsigned int index):
        normalTicks(_normalTicks), lockTicks(_lockTicks),
        index(index)
    {
        //
    }

    unsigned int getNormalTicks() { return normalTicks; }
    unsigned int getLockTicks() { return lockTicks; }
    unsigned int getIndex() { return index; }

protected:
    unsigned int normalTicks;
    unsigned int lockTicks;
    unsigned int index;
};
