#include <fused/type_a/micro.h>
#include <fused/type_a/microAppFactory.h>

MicroOSTypeA::MicroOSTypeA(Config *cfg) : MicroOS(cfg) {
    delete factory;
    factory = new TypeAMicroAppFactory();
    return;
}
