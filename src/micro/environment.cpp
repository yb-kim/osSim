#include <micro/environment.h>

MicroEnvironment::MicroEnvironment(EnvConfig *cfg) : Environment(cfg) {
    cores = new Core*[nCores];
    unsigned int startCoreIndex = cfg->getStartCoreIndex();
    for(int i=0; i<nCores; i++) {
        unsigned int coreIndex = i + startCoreIndex;
        cout << "initializing core" << coreIndex << endl;
        cores[i] = new Core(i, coreIndex, this);
    }
}
