#include <core.h>

class Environment {
public:
    Environment(int nCores);
    void createCores(int n);
    int getNCores() { return nCores; }
    Core* getCores() { return cores; }
private:
    int nCores;
    Core *cores;
};
