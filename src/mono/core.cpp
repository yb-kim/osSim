#include <mono/core.h>

#include <iostream>

using namespace std;

MonoCore::MonoCore(unsigned int index,
        Environment *env,
        unsigned int nSyscalls): Core(index, env) 
{
    cacheStates = new CacheState[nSyscalls];
    for(int i=0; i<nSyscalls; i++) {
        cacheStates[i] = INVALID;
    }
}
