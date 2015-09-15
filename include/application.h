#pragma once

#include <syscall.h>
#include <applicationSpec.h>
#include <environment.h>
#include <string>
#include <lib/rapidjson/document.h>

class Environment;
class Core;
class SyscallSpec;

class Application {
public:
    Application(int appSpecIndex);

    static unsigned int getNSpecs() { return nSpecs; }
    static ApplicationSpec* getAppSpec(int index) { return (index >= 0 ? appSpecs[index] : 0); }
    static int getSyscallProcessIndex(int syscallIndex) { 
        return nSpecs + syscallIndex;
    }
    static void setApplications(std::string appSpecs);

    bool isFinished() { return finished; }

    virtual void run(unsigned int unitTick) = 0;
    
    void setCoreIndex(unsigned int index);
    void setEnv(Environment *env);
    unsigned int getCoreIndex() { return coreIndex; }

    //ApplicationSpec* getAppSpec() { return spec; }

protected:
    //static variables
    static ApplicationSpec **appSpecs;
    static unsigned int nSpecs;
    static unsigned int nextId;

    //private functions
    virtual void setPC(unsigned int syscallIndex) = 0;
    bool moveToNextSyscall(); // returns false when there is no syscall remaining
    SyscallSpec* getCurrentSyscallSpec(int i=-1);
    static void readConfig(std::string appSpecConfig);

    //private variables
    static rapidjson::Document* specConfig;
    unsigned int id;
    int specIndex;
    ApplicationSpec *spec;
    unsigned int syscallPointer;
    bool finished;
    unsigned int coreIndex;
    Environment *env;
};
