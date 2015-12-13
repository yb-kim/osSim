#pragma once

#include <core.h>
#include <config.h>

#include <iostream>

using namespace std;

class OS;
class Core;

class Environment {
public:
    Environment(EnvConfig *cfg);
    void createCores(int n);
    int getNCores() { return nCores; }
    Core** getCores() { return cores; }
    Core* getCore(int n) { return cores[n]; }
    int getMessageCost(int src, int dest) {
        return (this->*costFunction)(src, dest); 
    }

    typedef enum {
        RING,
        AIM7
    } Topology;
protected:
    int nCores;
    Core **cores;
    OS *os;
    Topology topology;
    unsigned int baseLatency;
    unsigned int ipc_remote;
    unsigned int ipc_die;

    int (Environment::*costFunction) (int src, int dest);
    int getMessageCost_RING(int src, int dest);
    int getMessageCost_AIM7(int src, int dest);
};
