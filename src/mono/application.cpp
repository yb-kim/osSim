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
    state = NORMAL;
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
    getLock(specIndex);
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

void MonoApplication::setPC(unsigned int syscallIndex) {
    SyscallSpec *syscall = Syscall::getSyscallSpec(spec->getSyscallIndex()[syscallIndex]);
    pc.normalTicks = syscall->getNormalTicks();
    pc.lockTicks = syscall->getLockTicks();
}

bool MonoApplication::isSyscallFinished() {
    return ((pc.normalTicks == 0) && (pc.lockTicks == 0));
}

void MonoApplication::getLock(int specIndex) {
    MonoEnvironment *menv = (MonoEnvironment *)env;
    MonoCore *core = (MonoCore *)menv->getCore(coreIndex);
    MonoCore::CacheState cacheState = core->getCacheState(specIndex);

    //read cache line
    switch(cacheState) {
    case MonoCore::MODIFIED:
        break;

    case MonoCore::EXCLUSIVE:
        break;

    case MonoCore::SHARED:
        break;

    case MonoCore::INVALID:
        break;

    default:
        break;
    }

    //try to get lock

    //if got lock, write to cache line
}
