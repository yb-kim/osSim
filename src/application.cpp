#include <application.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;
using namespace rapidjson;

ApplicationSpec** Application::appSpecs;
unsigned int Application::nSpecs;

Application::Application() {
    //
}

Application::Application(unsigned int appSpecIndex):
    syscallPointer(0),
    finished(false)
{
    specIndex = appSpecIndex;
    spec = Application::getAppSpec(specIndex);
    setPC(syscallPointer);
}

void Application::run(unsigned int unitTick) {
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

void Application::setPC(unsigned int syscallIndex) {
    SyscallSpec *syscall = Syscall::getSyscallSpec(spec->getSyscallIndex()[syscallIndex]);
    pc.normalTicks = syscall->getNormalTicks();
    pc.lockTicks = syscall->getLockTicks();
}

unsigned int Application::processNormalTicks(unsigned int tick) {
    unsigned int processed = (tick < pc.normalTicks) ? tick : pc.normalTicks;
    pc.normalTicks -= processed;
    cout << processed << " normal ticks processed" << endl;
    return (tick-processed);
}

unsigned int Application::processLockTicks(unsigned int tick) {
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

bool Application::moveToNextSyscall() {
    if(syscallPointer == spec->getNSyscalls()-1) return false;
    syscallPointer++;
    setPC(syscallPointer);
    return true;
}

bool Application::isSyscallFinished() {
    return ((pc.normalTicks == 0) && (pc.lockTicks == 0));
}

void Application::setApplications(std::string appSpecConfig) {
    ifstream inFile(appSpecConfig.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());

    nSpecs = specs["types"].Size();
    appSpecs = new ApplicationSpec*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        ApplicationSpec::ApplicationType type;
        string typeString = specs["types"][i]["type"].GetString();
        if(typeString == "normal") type = ApplicationSpec::NORMAL;
        else {
            //default
            type = ApplicationSpec::NORMAL;
        }
        Value& syscalls = specs["types"][i]["syscalls"];
        SizeType nSyscalls = syscalls.Size();
        int *syscallIndex = new int[nSyscalls];
        for(SizeType j=0; j<nSyscalls; j++) syscallIndex[j] = syscalls[j].GetInt();
        appSpecs[i] = new ApplicationSpec(type, syscallIndex, nSyscalls);
    }
}
