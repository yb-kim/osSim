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
    else topology = RING;

    switch(topology) {
    case RING:
        costFunction = &Environment::getMessageCost_RING;
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
    return hops * baseLatency;
}
