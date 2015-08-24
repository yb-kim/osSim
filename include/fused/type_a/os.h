#pragma once

#include <fused/type_a/mono.h>
#include <fused/type_a/micro.h>

class FusedOSTypeA : public OS {
public:
    FusedOSTypeA(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    void executeCores();
private:
    MonoOSTypeA* monoOS;
    MicroOSTypeA* microOS;
    int nMonoCores;

    void setOsSpecificSpecs(std::string osSpecificSpecs);
};
