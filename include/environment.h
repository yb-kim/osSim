#pragma once

#include <core.h>
#include <config.h>

#include <iostream>

using namespace std;

class OS;

class Environment {
public:
    Environment(EnvConfig *cfg);
    void createCores(int n);
    int getNCores() { return nCores; }
    Core** getCores() { return cores; }
    Core* getCore(int n) { return cores[n]; }
protected:
    int nCores;
    Core **cores;
    OS *os;
};
