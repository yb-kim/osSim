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
    ((MicroAppFactory *)factory)->setOS(this);
    for(int i=0; i<nApps; i++) {
        readyQueue->enque(factory->createApp());
    }
    //TODO: set core 0 as NS
    env->getCore(0)->loadApp(new NSServiceApplication());
    //TODO: set os service cores
    for(int i=0; i<nSet; i++) {
        for(int j=1; j<nServices; j++) {
            int index = nServices*i+j;
            env->getCore(index)->loadApp(
                    new MicroServiceApplication(services[j]->type));
            services[j]->runningCoreIndex.push_back(index);
        }
    }
    for(int i=nSet*nServices; i<env->getNCores(); i++) {
        env->getCore(i)->loadApp(
                readyQueue->deque());
    }
}

MicroOS::Service* MicroOS::getService(ServiceType serviceType) {
    for(int i=0; i<nServices; i++) {
        if(services[i]->type == serviceType) {
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
    for(unsigned int i=0; i<env->getNCores(); i++) {
        Core *core = env->getCore(i);
        MicroApplication *app = (MicroApplication *)core->getAppRunning();
        app->setNSCacheExpiration(
                app->getNSCacheExpiration()-unitTick);
        if(app->isFinished()) {
            nAppsFinished++;
            delete app;
            readyQueue->enque(factory->createApp());
            core->loadApp(readyQueue->deque());
        }
    }
    return;
}
