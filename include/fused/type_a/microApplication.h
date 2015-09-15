#pragma once

#include <micro/os.h>
#include <micro/application.h>
#include <fused/type_a/os.h>

class TypeAMicroApplication : public MicroApplication {
public:
    TypeAMicroApplication(unsigned int appSpecIndex);
    void setFusedOS(FusedOSTypeA *os) { fusedOS = os; }

protected:
    FusedOSTypeA* fusedOS;
    virtual void doIpc();
    bool isSyscallFinished();
};
