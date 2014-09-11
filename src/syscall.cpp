#include <mono/syscall.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace rapidjson;

SyscallSpec** Syscall::syscalls;
Application** Syscall::locks;
Document* Syscall::specConfig;

SyscallSpec* Syscall::getSyscallSpec(unsigned int n) {
    return syscalls[n];
}

void Syscall::readConfig(string syscallSpecs) {
    //Read and parse config file
    ifstream inFile(syscallSpecs.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());
    specConfig = new Document();
    specConfig->Parse(jsonConfig.c_str());
}

void Syscall::setMonoSyscalls(string syscallSpecs) {
    readConfig(syscallSpecs);

    //Initialize each syscalls
    SizeType nSpecs = (*specConfig)["types"].Size();
    syscalls = (SyscallSpec **)new MonoSyscallSpec*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        Value& ticks = (*specConfig)["types"][i]["ticks"];
        unsigned int normalTicks = ticks["normal"].GetInt();
        unsigned int lockTicks = ticks["lock"].GetInt();
        syscalls[i] = new SyscallSpec(normalTicks, lockTicks);
    }

    //Initialize locks
    locks = new Application*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) locks[i] = NULL;
}

void Syscall::setMicroSyscalls(string syscallSpecs) {
    readConfig(syscallSpecs);
}

bool Syscall::getLock(unsigned int syscallNum, Application *app) {
    if(locks[syscallNum]==NULL || locks[syscallNum]==app) {
        locks[syscallNum] = app;
        return true;
    }
    return false;
}

void Syscall::freeLock(unsigned int syscallNum, Application *app) {
    if(locks[syscallNum]==app) locks[syscallNum] = NULL;
}
