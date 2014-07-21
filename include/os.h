#include <config.h>
#include <environment.h>
#include <appFactory.h>

class OS {
public:
    OS(Config *cfg);
    virtual void run() = 0;

protected:
    Environment *env;
    AppFactory *factory;
    unsigned int maxTick;
    unsigned int curTick;
    unsigned int unitTick;
};
