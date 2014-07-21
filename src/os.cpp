#include <os.h>

OS::OS(Config *cfg) :
    maxTick(cfg->getMaxTick()),
    unitTick(cfg->getUnitTick())
{
    env = new Environment(cfg->getNCores());
}
