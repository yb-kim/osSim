#include <iostream>
#include <string>
#include <environment.h>

using namespace std;

Environment::Environment(EnvConfig *cfg):
    nCores(cfg->getNCores()),
    baseLatency(cfg->getBaseLatency())
{
    string topologyName = cfg->getTopology();
    if(topologyName == "RING") topology = RING;
    else if(topologyName == "AIM7") topology = AIM7;
    else topology = RING;

    switch(topology) {
    case RING:
        costFunction = &Environment::getMessageCost_RING;
        break;
    case AIM7:
        costFunction = &Environment::getMessageCost_AIM7;
        break;
    default:
        string defaultTopologyName = "RING";
        int (Environment::*defaultCostFunction) (int, int) =
            &Environment::getMessageCost_RING;
        cout << "warning: unknown topology name. Set to " << 
            defaultTopologyName << "." << endl;
        costFunction = defaultCostFunction;
    }
}

int Environment::getMessageCost_RING(int src, int dest) {
    unsigned int hops = src > dest ? src - dest : dest - src;
    unsigned int maxHops = nCores / 2;
    hops = hops > maxHops ? nCores - hops : hops;
    cout << "src, dest, nCores, hops: " << src << " " << dest << " " << nCores << " " << hops << " " << endl;
    return hops / 100 * baseLatency;
    return 10;
}

int Environment::getMessageCost_AIM7(int src, int dest) {
    int p = rand() % 5;
    if(p == 0) return ipc_remote;
    else return ipc_die;
}
