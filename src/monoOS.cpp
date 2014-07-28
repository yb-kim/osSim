#include <iostream>
#include <monoOS.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {

}

void MonoOS::run() {
    while(curTick <= maxTick) {
        cout << "current Tick: " << curTick << endl;
        Core* cores = env->getCores();
        for(int i=0; i<env->getNCores(); i++) {
            //
        }
        curTick += unitTick;
    }
}
