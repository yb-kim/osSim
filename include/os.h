#include <config.h>
#include <environment.h>
#include <appFactory.h>
#include <appQueue.h>

class OS {
public:
    OS(Config *cfg);
    virtual void run() = 0;

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
};
