#pragma once

#include <fused/type_a/mono.h>
#include <fused/type_a/micro.h>
#include <fused/type_a/request.h>
#include <fused/type_a/microApplication.h>
#include <queue>
#include <list>

class MicroOSTypeA;
class MonoOSTypeA;

class FusedOSTypeA : public OS {
public:
    FusedOSTypeA(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    void executeCores();
    void sendIpc(FusedOSRequest *request);
    void processIpc(int unitTick);
private:
    MonoOSTypeA* monoOS;
    MicroOSTypeA* microOS;
    int nMonoCores;
    int remainingTicks;
    int getIpcCost(int srcCoreIndex, int destCoreIndex);
    queue<FusedOSRequest *, list<FusedOSRequest *> > requestQueue;

    void setOsSpecificSpecs(std::string osSpecificSpecs);
    int getGlobalCoreIndex(int microCoreIndex) { return microCoreIndex + nMonoCores; }
    void wrapUp();
};
