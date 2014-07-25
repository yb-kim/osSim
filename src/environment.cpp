#include <iostream>
#include <environment.h>

using namespace std;

Environment::Environment(EnvConfig *cfg):
    nCores(cfg->getNCores())
{
    cores = new Core[nCores];
}
