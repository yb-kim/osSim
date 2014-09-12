#include <mono/application.h>
#include <iostream>
#include <string>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>

using namespace rapidjson;
using namespace std;

MonoApplication::MonoApplication(unsigned int appSpecIndex) : Application(appSpecIndex) {
    //
}

void MonoApplication::setApplications(std::string appSpecConfig) {
    readConfig(appSpecConfig);

    nSpecs = (*specConfig)["types"].Size();
    appSpecs = new ApplicationSpec*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        ApplicationSpec::ApplicationType type;
        string typeString = (*specConfig)["types"][i]["type"].GetString();
        if(typeString == "normal") type = ApplicationSpec::NORMAL;
        else {
            //default
            type = ApplicationSpec::NORMAL;
        }
        Value& syscalls = (*specConfig)["types"][i]["syscalls"];
        SizeType nSyscalls = syscalls.Size();
        int *syscallIndex = new int[nSyscalls];
        for(SizeType j=0; j<nSyscalls; j++) syscallIndex[j] = syscalls[j].GetInt();
        appSpecs[i] = new ApplicationSpec(type, syscallIndex, nSyscalls);
    }
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
