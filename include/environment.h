#include <core.h>

class Environment {
public:
    Environment(int nCores);
    void createCores(int n);
private:
    int nCores;
    Core *cores;
};
