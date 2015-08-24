#include <mono/core.h>

#include <iostream>

using namespace std;

MonoCore::MonoCore(unsigned int index,
        Environment *env,
        unsigned int nSyscalls): Core(index, env) 
{
    printingCoreIndex = index;
    cacheStates = new CacheState[nSyscalls];
    for(int i=0; i<nSyscalls; i++) {
        cacheStates[i] = INVALID;
    }
}

void MonoCore::setCacheState(unsigned int index,
        CacheState state) { 
    cacheStates[index] = state; 
}
