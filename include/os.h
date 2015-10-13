#pragma once

#include <config.h>
#include <environment.h>
#include <appFactory.h>
#include <appQueue.h>
#include <string>

class OS {
public:
    OS(Config *cfg);
    void run();
    virtual void init();
    virtual void executeCore(unsigned int n, unsigned int unitTick);
    virtual void checkAndDoSchedule() = 0;
    virtual void afterExecute() = 0;
    virtual void wrapUp();
    virtual void executeCores();

    void increaseCurTick(int unitTick) { curTick += unitTick; }

    Environment* getEnv() { return env; }

    unsigned int getNAppsFinished() { return nAppsFinished; }

protected:
    Environment *env;
    AppFactory *factory;
    AppQueue *readyQueue;

    //configurations
    OsConfig *osConfig;
    unsigned int maxTick;
    unsigned int curTick;
    unsigned int unitTick;
    unsigned int contextSwitchTick;
    int untilContextSwitch;
    unsigned int nApps;
    std::string configFileRoot;
    std::string osTypeString;
    std::string syscallSpecs;
    std::string appSpecs;
    std::string osSpecificSpecs;
    int nWeights;
    int *appWeights;

    //statistics
    unsigned int nAppsFinished;
};
