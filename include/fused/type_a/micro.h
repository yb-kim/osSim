#pragma once

#include <micro/os.h>
#include <fused/type_a/os.h>
#include <fused/type_a/request.h>

class FusedOSTypeA;
class TypeAMicroApplication;

class MicroOSTypeA : public MicroOS {
public:
    MicroOSTypeA(Config *cfg);
    void setFusedOS(FusedOSTypeA *os) { fusedOS = os; }
    void sendIpc(FusedOSRequest *request);
    void resumeApplication(int coreIndex);
protected:
    FusedOSTypeA *fusedOS;
};
