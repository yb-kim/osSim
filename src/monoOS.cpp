#include <iostream>
#include <monoOS.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {

}

void MonoOS::run() {
    while(curTick <= maxTick) {
        cout << "tick: " << curTick << endl;
        Core* cores = env->getCores();
        for(int i=0; i<env->getNCores(); i++) {
            cout << "Hello core " << i << endl;
        }
        curTick += unitTick;
    }
}
