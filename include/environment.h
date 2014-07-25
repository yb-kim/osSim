#include <core.h>
#include <config.h>

class Environment {
public:
    Environment(EnvConfig *cfg);
    void createCores(int n);
    int getNCores() { return nCores; }
    Core* getCores() { return cores; }
private:
    int nCores;
    Core *cores;
};
