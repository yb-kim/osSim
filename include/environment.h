#include <core.h>
#include <config.h>

class Environment {
public:
    Environment(EnvConfig *cfg);
    void createCores(int n);
    int getNCores() { return nCores; }
    Core** getCores() { return cores; }
    Core* getCore(int n) { return cores[n]; }
private:
    int nCores;
    Core **cores;
};
