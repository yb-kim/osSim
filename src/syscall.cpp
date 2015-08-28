#include <mono/syscall.h>
#include <micro/syscall.h>
#include <micro/os.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;
using namespace rapidjson;

SyscallSpec** Syscall::syscalls;
Application** Syscall::locks;
Document* Syscall::specConfig;
unsigned int Syscall::nSpecs;
Syscall::SyscallNameIndexPair* Syscall::syscallNameIndexPairs;

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
    nSpecs = (*specConfig)["types"].Size();
    syscalls = (SyscallSpec **)new MonoSyscallSpec*[nSpecs];
    Syscall::syscallNameIndexPairs = new SyscallNameIndexPair[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        Value& workloadParams = (*specConfig)["types"][i]["workload"];
        SizeType nWorkloads = workloadParams.Size();
        string name = (*specConfig)["types"][i]["name"].GetString();
        MonoSyscallSpec::SyscallWorkload *workloads = 
            new MonoSyscallSpec::SyscallWorkload[nWorkloads];
        for(SizeType j=0; j<nWorkloads; j++) {
            MonoSyscallSpec::SyscallWorkload workload = workloads[j];
            string workloadType = workloadParams[j]["type"].GetString();
            if(workloadType == "normal")
                workload.type = MonoSyscallSpec::NORMAL;
            else
                workload.type = MonoSyscallSpec::LOCK;
            unsigned int tick = workloadParams[j]["tick"].GetInt();
            workload.tick = tick;
        }
        //syscalls[i] = new MonoSyscallSpec(normalTicks, lockTicks, name, i);
        syscalls[i] = new MonoSyscallSpec(workloads, name, i);
        Syscall::syscallNameIndexPairs[i].name = name;
        Syscall::syscallNameIndexPairs[i].index = i;
    }

    //Initialize locks
    locks = new Application*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) locks[i] = NULL;
}

void Syscall::setMicroSyscalls(string syscallSpecs) {
    readConfig(syscallSpecs);

    SizeType nSpecs = (*specConfig)["types"].Size();
    syscalls = (SyscallSpec **)new MicroSyscallSpec*[nSpecs];
    Syscall::syscallNameIndexPairs = new SyscallNameIndexPair[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        Value& syscallConfig = (*specConfig)["types"][i];
        unsigned int normalTicks = syscallConfig["normalTicks"].GetInt();
        SizeType nServices = syscallConfig["services"].Size();
        string name = syscallConfig["name"].GetString();
        MicroOS::ServiceType *services = new MicroOS::ServiceType[nServices];
        for(SizeType j=0; j<nServices; j++) {
            string serviceType = syscallConfig["services"][j].GetString();
            services[j] = MicroOS::getServiceType(serviceType);
        }
        syscalls[i] = new MicroSyscallSpec(normalTicks, services, nServices, name, i);
        Syscall::syscallNameIndexPairs[i].name = name;
        Syscall::syscallNameIndexPairs[i].index = i;
    }
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


bool Syscall::checkLock(unsigned int syscallNum, Application *app) {
    if(locks[syscallNum]==NULL || locks[syscallNum]==app) {
        return true;
    }
    return false;
}


bool Syscall::isTakingLock(unsigned int syscallNum, Application *app) {
    return locks[syscallNum]==app;
}


int Syscall::getSyscallIndexByName(string syscallName) {
    for(int i=0; i<Syscall::nSpecs; i++) {
        cout << "syscallNameIndexPair: " << syscallNameIndexPairs[i].name << ", " << syscallNameIndexPairs[i].index << endl;
        if(Syscall::syscallNameIndexPairs[i].name == syscallName) 
            return syscallNameIndexPairs[i].index;
    }
    return -1;
}
