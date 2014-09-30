#pragma once

#include <syscall.h>
#include <applicationSpec.h>
#include <string>
#include <lib/rapidjson/document.h>

class Application {
public:
    Application(int appSpecIndex);

    static unsigned int getNSpecs() { return nSpecs; }
    static ApplicationSpec* getAppSpec(int index) { return (index >= 0 ? appSpecs[index] : 0); }
    static void setApplications(std::string appSpecs);

    bool isFinished() { return finished; }

    virtual void run(unsigned int unitTick) = 0;
    
    void setCoreIndex(unsigned int index) { coreIndex = index; }
    unsigned int getCoreIndex() { return coreIndex; }

protected:
    //static variables
    static ApplicationSpec **appSpecs;
    static unsigned int nSpecs;

    //private functions
    virtual void setPC(unsigned int syscallIndex);
    bool moveToNextSyscall(); // returns false when there is no syscall remaining
    bool isSyscallFinished();
    static void readConfig(std::string appSpecConfig);

    //private variables
    static rapidjson::Document* specConfig;
    int specIndex;
    ApplicationSpec *spec;
    struct PC {
        unsigned int normalTicks;
        unsigned int lockTicks;
    } pc;
    unsigned int syscallPointer;
    bool finished;
    unsigned int coreIndex;
};
