#pragma once

#include <syscall.h>
#include <applicationSpec.h>

class Application {
public:
    Application(unsigned int appSpecIndex);

    static void setApplications(std::string appSpecs);
    static unsigned int getNSpecs() { return nSpecs; }
    static ApplicationSpec* getAppSpec(unsigned int index) { return appSpecs[index]; }

    bool isFinished() { return finished; }

    virtual void run(unsigned int unitTick) = 0;

protected:
    //static variables
    static ApplicationSpec **appSpecs;
    static unsigned int nSpecs;
    //private functions
    void setPC(unsigned int syscallIndex);
    bool moveToNextSyscall(); // returns false when there is no syscall remaining
    bool isSyscallFinished();
    //bool isFinished() { return (syscallPointer == spec->getNSyscalls()) && untilSyscallFinish == 0; }
    //private variables
    unsigned int specIndex;
    ApplicationSpec *spec;
    struct PC {
        unsigned int normalTicks;
        unsigned int lockTicks;
    } pc;
    unsigned int syscallPointer;
    bool finished;
};
