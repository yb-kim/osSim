#include <mono/environment.h>
#include <mono/core.h>

#include <iostream>

using namespace std;

MonoEnvironment::MonoEnvironment(EnvConfig *cfg, OS *os) : Environment(cfg),
    os(os)
{
    cores = (Core **)(new MonoCore*[nCores]);
    unsigned int nSyscalls = Syscall::getNSpecs();
    for(int i=0; i<nCores; i++) {
        cout << "initializing core" << i << endl;
        cores[i] = new MonoCore(i, this, nSyscalls);
    }
    bus = new Bus();
}
