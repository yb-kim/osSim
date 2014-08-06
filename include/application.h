#pragma once

#include <syscall.h>

class ApplicationSpec;

class Application {
public:
    Application();
    static void setApplications(std::string appSpecs);
private:
    static ApplicationSpec **appSpecs;
    //
};


class ApplicationSpec {
public:
    enum ApplicationType { 
        NORMAL 
    };

    ApplicationSpec(ApplicationType _type, int *_syscallIndex):
        type(_type),
        syscallIndex(_syscallIndex)
    {
        //
    }
    
private:
    ApplicationType type;
    int *syscallIndex;
};
