#include <os.h>

OS::OS(Config *cfg) :
    osConfig(cfg->getOsConfig()),
    maxTick(osConfig->getMaxTick()),
    unitTick(osConfig->getUnitTick()),
    contextSwitchTick(osConfig->getContextSwitchTick())
{
    curTick = 0;
    env = new Environment(cfg->getEnvConfig());
    readyQueue = new AppQueue();
}
