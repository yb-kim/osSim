#include <micro/environment.h>

MicroEnvironment::MicroEnvironment(EnvConfig *cfg) : Environment(cfg) {
    cores = new Core*[nCores];
    for(int i=0; i<nCores; i++) {
        cout << "initializing core" << i << endl;
        cores[i] = new Core(i, this);
    }
}
