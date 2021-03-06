#pragma once

#include <core.h>

class MonoCore : public Core {
public:
    MonoCore(unsigned int index, Environment *env, 
            unsigned int nSyscalls);

    typedef enum {
        MODIFIED,
        EXCLUSIVE,
        SHARED,
        INVALID
    } CacheState;

    CacheState getCacheState(unsigned int index) { return cacheStates[index]; }
    void setCacheState(unsigned int index, CacheState state);

private:
    CacheState *cacheStates;
};
