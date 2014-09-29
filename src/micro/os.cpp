#include <micro/os.h>
#include <micro/appFactory.h>
#include <micro/application.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace rapidjson;
using namespace std;

MicroOS::Service** MicroOS::services;
int MicroOS::nSet;
int MicroOS::nServices;

MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    setOsSpecificSpecs(osSpecificSpecs);
    factory = new MicroAppFactory();
    Syscall::setMicroSyscalls(syscallSpecs);
}

void MicroOS::init() {
    //OS::init();
    //TODO: set core 0 as NS
    env->getCore(0)->loadApp(new NSServiceApplication());
    //TODO: set os service cores
    for(int i=FM; i<SERVICES_END; i++) {
        env->getCore(i)->loadApp(
                new MicroServiceApplication((ServiceType)i));
    }
}

MicroOS::Service* MicroOS::getService(ServiceType serviceType) {
    for(int i=0; i<nServices; i++) {
        if(services[i]->type == (ServiceType)i) {
            return services[i];
        }
    }
    return (Service *)NULL;
}


void MicroOS::setOsSpecificSpecs(std::string osSpecificSpecs) {
    ifstream inFile(osSpecificSpecs.c_str());
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string jsonConfig(buffer.str());
    Document specs;
    specs.Parse(jsonConfig.c_str());
    Document specConfig;
    specConfig.Parse(jsonConfig.c_str());

    nSet = specConfig["nSet"].GetInt();
    nServices = specConfig["services"].Size();
    services = new Service*[nServices];
    for(SizeType i=0; i<nServices; i++) {
        Value& item = specConfig["services"][i];
        string name = item["name"].GetString();
        SizeType ticks = item["ticks"].GetInt();
        ServiceType type = getServiceType(name);
        services[i] = new Service(type, ticks);
    }
}


void MicroOS::checkAndDoSchedule() {
    return;
}

void MicroOS::afterExecute() {
    return;
}
