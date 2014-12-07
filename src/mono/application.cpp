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
    cout << "Running app #" << id << endl;
    if(isFinished()) return;
    //print syscall list of this application
    cout << "Syscalls: ";
    for(int i=0; i<spec->getNSyscalls(); i++) {
        //int *syscallIndex = spec->getSyscallIndex();
        MonoSyscallSpec *syscallSpec = (MonoSyscallSpec *) getCurrentSyscallSpec(i);
        cout << syscallSpec->getName();
        if(syscallPointer == i) cout << "(*)";
        cout << " -> ";
    }
    cout << "TERMINATE" << endl;
    if(state == WAITING_COHERENCY_REQUEST) {
        cout << "waiting coherency request to be finished..." << endl;
        return;
    }
    unsigned int remaining = unitTick;
    if(unitTick > 0) {
        remaining = processNormalTicks(unitTick);
    }
    if(remaining > 0) {
        remaining = processLockTicks(remaining);
    }
    if(remaining >= 0 && isSyscallFinished()) {
        bool next = moveToNextSyscall();
        if(!next) finished = true;
        else run(remaining);
    }
}

unsigned int MonoApplication::processNormalTicks(unsigned int tick) {
    unsigned int processed = (tick < pc.normalTicks) ? tick : pc.normalTicks;
    pc.normalTicks -= processed;
    cout << "normal tick processed: " << processed <<
        " (" << pc.normalTicks << " ticks remaining)" << endl;
    return (tick-processed);
}

unsigned int MonoApplication::processLockTicks(unsigned int tick) {
    cout << "start to process lock ticks" << endl;
    unsigned int syscallIndex = getCurrentSyscallSpec()->getIndex();
    bool success = getLock(syscallIndex);
    if(!success) {
        return 0;
    }
    unsigned int processed = (tick < pc.lockTicks) ? tick : pc.lockTicks;
    pc.lockTicks -= processed;
    cout << "lock tick processed: " << processed <<
        " (" << pc.lockTicks << " ticks remaining)" << endl;
    if(pc.lockTicks == 0) Syscall::freeLock(syscallIndex, this);
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

bool MonoApplication::getLock(int specIndex) {
    MonoEnvironment *menv = (MonoEnvironment *)env;
    MonoCore *core = (MonoCore *)menv->getCore(coreIndex);
    MonoCore::CacheState cacheState = core->getCacheState(specIndex);
    MonoSyscallSpec *syscallSpec = (MonoSyscallSpec *)getCurrentSyscallSpec();
    Bus *bus = menv->getBus();

    //read cache line
    switch(cacheState) {
    case MonoCore::MODIFIED:
        cout << "Cache line is in MODIFIED state" << endl;
        break;

    case MonoCore::EXCLUSIVE:
        cout << "Cache line is in EXCLUSIVE state" << endl;
        break;

    case MonoCore::SHARED:
        cout << "Cache line is in SHARED state" << endl;
        break;

    case MonoCore::INVALID:
        //send request to bus
        cout << "cache line is in INVALID state" << endl;
        CoherencyRequest *req;
        req = new CoherencyRequest(
                CoherencyRequest::READ,
                core,
                syscallSpec->getIndex());
        cout << "send coherency request to the bus" << endl;
        bus->enque(req);
        state = WAITING_COHERENCY_REQUEST;
        cout << "waiting the cache block is coming..." << endl;
        return false;
        break;

    default:
        break;
    }

    //try to get lock
    bool success = Syscall::checkLock(specIndex, this);

    //if got lock, write to cache line
    if(success) {
        cout << "The lock is not taken by other processes; " <<
            "try to take the lock" << endl;
        bool lockAquired = false;

        switch(cacheState) {
        case MonoCore::MODIFIED:
            lockAquired = Syscall::getLock(specIndex, this);
            //lockAquired must be true
            if(!lockAquired) {
                cout << "Error: lock aquisition in MODIFED failed" << endl;
            } else {
                cout << "Lock taken" << endl;
            }
            return true;

        case MonoCore::EXCLUSIVE:
            lockAquired = Syscall::getLock(specIndex, this);
            //lockAquired must be true
            if(!lockAquired) {
                cout << "Error: lock aquisition in EXCLUSIVE failed" << endl;
            }
            core->setCacheState(specIndex, MonoCore::MODIFIED);
            return true;

        case MonoCore::SHARED:
            CoherencyRequest *req;
            req = new CoherencyRequest(
                    CoherencyRequest::INVALIDATION,
                    core,
                    syscallSpec->getIndex());
            cout << "send coherency request to bus" << endl;
            bus->enque(req);
            state = WAITING_COHERENCY_REQUEST;
            cout << "waiting the cache block is coming..." << endl;
            return false;

        case MonoCore::INVALID:
            cout << "Error: cache state must not be in INVALID state when write" << endl;
            return false;

        defalut:
            break;
        }

    } else {
        //wait to aquire lock
        cout << "The lock is taken by another process" << endl;
        return false;
    }

    return false;
}


bool MonoApplication::snoopBus(CoherencyRequest *req) {
    unsigned int syscallIndex = req->syscallIndex;
    MonoEnvironment *menv = (MonoEnvironment *)env;
    MonoCore *core = (MonoCore *)menv->getCore(coreIndex);
    MonoCore::CacheState cacheState = core->getCacheState(syscallIndex);

    if(req->requestType == CoherencyRequest::READ) {
        switch(cacheState) {
        case MonoCore::MODIFIED:
        case MonoCore::EXCLUSIVE:
        case MonoCore::SHARED: {
            cout << "core " << core->getIndex() << " snooped the request and " <<
                "changed the state to SHARED" << endl;
            cout << "request: " << req->getDescription() << endl;
            req->src->setCacheState(syscallIndex, MonoCore::SHARED);
            core->setCacheState(syscallIndex, MonoCore::SHARED);
            MonoApplication *app = (MonoApplication *)req->src->getAppRunning();
            app->setState(NORMAL);
            return true;
        }

        case MonoCore::INVALID:
            return false;

        default:
            return false;
        }
    } else { //INVALIDATION
        if(req->src == core) return false;
        if(core->getCacheState(syscallIndex) == 
                MonoCore::INVALID) return false;
        cout << "core " << core->getIndex() << " invalidated "
            "the block of syscall " << syscallIndex << endl;
        core->setCacheState(syscallIndex, MonoCore::INVALID);
        //return false to deliver this request to next core
        return false;
    }
}
