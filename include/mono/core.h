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

private:
    CacheState *cacheStates;
};
