#include <application.h>
#include <iostream>
#include <lib/rapidjson/document.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace rapidjson;

ApplicationSpec** Application::appSpecs;
unsigned int Application::nSpecs;
Document* Application::specConfig;

Application::Application(unsigned int appSpecIndex):
    syscallPointer(0),
    finished(false)
{
    specIndex = appSpecIndex;
    spec = Application::getAppSpec(specIndex);
    setPC(syscallPointer);
}

void Application::setPC(unsigned int syscallIndex) {
    SyscallSpec *syscall = Syscall::getSyscallSpec(spec->getSyscallIndex()[syscallIndex]);
    pc.normalTicks = syscall->getNormalTicks();
    pc.lockTicks = syscall->getLockTicks();
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


void Application::readConfig(string appSpecConfig) {
    ifstream inFile(appSpecConfig.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    specConfig = new Document();
    specConfig->Parse(jsonConfig.c_str());
}
