#include <application.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;
using namespace rapidjson;

ApplicationSpec** Application::appSpecs;

Application::Application() {
    //
}

void Application::setApplications(std::string appSpecConfig) {
    ifstream inFile(appSpecConfig.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());

    SizeType nSpecs = specs["types"].Size();
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
        appSpecs[i] = new ApplicationSpec(type, syscallIndex);
    }
}
