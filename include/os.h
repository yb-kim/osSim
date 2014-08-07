#include <config.h>
#include <environment.h>
#include <appFactory.h>
#include <appQueue.h>

class OS {
public:
    OS(Config *cfg);
    void run();
    virtual void init();
    virtual void executeCore(unsigned int n, unsigned int unitTick);
    virtual void checkAndDoSchedule();
    virtual void afterExecute();
    virtual void wrapUp();

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

    //statistics
    unsigned int nAppsFinished;
};
