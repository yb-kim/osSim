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

Application::Application(int appSpecIndex):
    syscallPointer(0),
    finished(false)
{
    specIndex = appSpecIndex;
    spec = Application::getAppSpec(specIndex);
}

bool Application::moveToNextSyscall() {
    if(syscallPointer == spec->getNSyscalls()-1) return false;
    cout << "syscall moved from " << syscallPointer << " to " << syscallPointer+1 << endl;
    syscallPointer++;
    setPC(syscallPointer);
    return true;
}

void Application::readConfig(string appSpecConfig) {
    ifstream inFile(appSpecConfig.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    specConfig = new Document();
    specConfig->Parse(jsonConfig.c_str());
}

void Application::setApplications(std::string appSpecConfig) {
    readConfig(appSpecConfig);

    nSpecs = (*specConfig)["types"].Size();
    cout << "Reading application configurations (" << nSpecs <<
        " types of applications)" << endl;
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


void Application::setCoreIndex(unsigned int index) {
    coreIndex = index;
}


void Application::setEnv(Environment *env) {
    this->env = env;
}
