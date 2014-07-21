#include <iostream>
#include <monoOS.h>

using namespace std;

MonoOS::MonoOS(Config *cfg) : OS(cfg) {

}

void MonoOS::run() {
    while(curTick <= maxTick) {
        cout << curTick << endl;
        curTick += unitTick;
    }
}
