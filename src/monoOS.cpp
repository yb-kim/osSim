#include <iostream>
#include <monoOS.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {

}

void MonoOS::init() {
    OS::init();
}
