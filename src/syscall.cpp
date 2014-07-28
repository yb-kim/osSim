#include <syscall.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace rapidjson;

SyscallSpec** Syscall::syscalls;

SyscallSpec* Syscall::getSyscallSpec(unsigned int n) {
    return syscalls[n];
}

void Syscall::setSyscalls(string syscallSpecs) {
    //Read and parse config file
    ifstream inFile(syscallSpecs.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());

    //Initialize each syscalls
    SizeType nSpecs = specs["types"].Size();
    syscalls = new SyscallSpec*[nSpecs];
    for(SizeType i=0; i<nSpecs; i++) {
        Value& ticks = specs["types"][i]["ticks"];
        unsigned int normalTicks = ticks["normal"].GetInt();
        unsigned int lockTicks = ticks["lock"].GetInt();
        syscalls[i] = new SyscallSpec(normalTicks, lockTicks);
    }
}
