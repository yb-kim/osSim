#include <iostream>
#include <environment.h>

using namespace std;

Environment::Environment(EnvConfig *cfg):
    nCores(cfg->getNCores())
{
    cores = new Core*[nCores];
    for(int i=0; i<nCores; i++) cores[i] = new Core(i);
}
