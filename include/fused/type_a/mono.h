#pragma once

#include <mono/os.h>
#include <fused/type_a/os.h>

class FusedOSTypeA;

class MonoOSTypeA : public MonoOS {
public:
    MonoOSTypeA(Config *cfg);
    void checkAndDoSchedule();
    void init();
    void makeAppFactory();
    void setFusedOS(FusedOSTypeA *os) { fusedOS = os; }
    void addAppToOS(int syscallIndex, int srcCoreIndex);
    void processFinishedApp(int coreIndex);
    //
protected:
    FusedOSTypeA *fusedOS;
};
