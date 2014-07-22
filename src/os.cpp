#include <os.h>

OS::OS(Config *cfg) :
    maxTick(cfg->getMaxTick()),
    unitTick(cfg->getUnitTick())
{
    curTick = 0;
    env = new Environment(cfg->getNCores());
}
