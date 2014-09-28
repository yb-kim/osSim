#include <mono/application.h>
#include <iostream>
#include <string>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>

using namespace rapidjson;
using namespace std;

MonoApplication::MonoApplication(unsigned int appSpecIndex) : Application(appSpecIndex) {
    setPC(syscallPointer);
}

void MonoApplication::run(unsigned int unitTick) {
    if(isFinished()) return;
    unsigned int remaining;
    remaining = processNormalTicks(unitTick);
    remaining = processLockTicks(remaining);
    if(remaining >= 0 && isSyscallFinished()) {
        bool next = moveToNextSyscall();
        if(!next) finished = true;
        else run(remaining);
    }
}

unsigned int MonoApplication::processNormalTicks(unsigned int tick) {
    unsigned int processed = (tick < pc.normalTicks) ? tick : pc.normalTicks;
    pc.normalTicks -= processed;
    cout << processed << " normal ticks processed" << endl;
    return (tick-processed);
}

unsigned int MonoApplication::processLockTicks(unsigned int tick) {
    if(!Syscall::getLock(specIndex, this)) {
        cout << "cannot gain lock" << endl;
        return 0;
    }
    unsigned int processed = (tick < pc.lockTicks) ? tick : pc.lockTicks;
    pc.lockTicks -= processed;
    cout << processed << " lock ticks processed" << endl;
    if(pc.lockTicks == 0) Syscall::freeLock(specIndex, this);
    return (tick-processed);
}

void MonoApplication::freeLock() {
    Syscall::freeLock(specIndex, this);
}
