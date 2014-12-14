#include <micro/os.h>
#include <micro/appFactory.h>
#include <micro/application.h>
#include <micro/environment.h>
#include <lib/rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace rapidjson;
using namespace std;

MicroOS::Service** MicroOS::services;
int MicroOS::nSet;
int MicroOS::nServices;


MicroOS::MicroOS(Config *cfg) : OS(cfg) {
    setOsSpecificSpecs(osSpecificSpecs);
    env = new MicroEnvironment(cfg->getEnvConfig());
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
    env->getCore(0)->loadApp(new NSServiceApplication(this));
    //TODO: set os service cores
    cout << "nSet = " << nSet << endl;
    for(int i=0; i<nSet; i++) {
        for(int j=1; j<nServices; j++) {
            int index = (nServices-1)*i+j;
            cout << "loading " << getServiceTypeString(services[j]->type) << 
                " service on core " << index << endl;
            env->getCore(index)->loadApp(
                    new MicroServiceApplication(services[j]->type, this));
            services[j]->runningCoreIndex.push_back(index);
        }
    }
    for(int i=nSet*(nServices-1)+1; i<env->getNCores(); i++) { //Exclude NS in nServices
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
    nsCacheExpiration = specConfig["nsCacheExpiration"].GetInt();
    ipcCost_die = specConfig["ipcCost_die"].GetInt();
    ipcCost_hop = specConfig["ipcCost_hop"].GetInt();
    ipcCost_2hops = specConfig["ipcCost_2hops"].GetInt();
}


void MicroOS::checkAndDoSchedule() {
    list< RequestEntry *>::iterator iter = waitingRequests.begin();
    list< RequestEntry *>::iterator iterEnd = waitingRequests.end();
    while(iter != waitingRequests.end()) {
        RequestEntry *entry = *iter;
        entry->ticks -= unitTick;
        if(entry->ticks <= 0) {
            sendRequest(entry->req);
            delete entry;
            iter = waitingRequests.erase(iter);
            continue;
        }
        iter++;
    }
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

void MicroOS::getRequest(Request *req) {
    RequestEntry *entry = new RequestEntry();
    entry->ticks = getIpcTicks(req);
    entry->req = req;
    waitingRequests.size();
    waitingRequests.push_back(entry);
    cout << "Request is sent; the request takes " << entry->ticks << 
        " ticks to be recieved" << endl;
    return;
}

int MicroOS::getIpcTicks(Request *req) {
    switch(rand()%3) {
    case 0:
        return ipcCost_die;
    case 1:
        return ipcCost_hop;
    case 2:
        return ipcCost_2hops;
    default:
        return ipcCost_die;
    }
}

void MicroOS::sendRequest(Request *req) {
    unsigned int coreIndex = *(req->currentService);
    MicroApplication *src = (MicroApplication *)(
            getEnv()->getCore(coreIndex)->getAppRunning());
    (req->currentService)++;
    coreIndex = *(req->currentService);
    MicroApplication *dest = (MicroApplication *)(
            getEnv()->getCore(coreIndex)->getAppRunning());
    if(dynamic_cast<MicroServiceApplication *>(dest)) {
        MicroServiceApplication *target = (MicroServiceApplication *)dest;
        cout << "Core " << dest->getCoreIndex() << "(" <<
            getServiceTypeString(target->getService()->type) <<
            ") recieved the request from core " << 
            src->getCoreIndex() << endl;
        target->enque(req);
    } else {
        //if the request is sent from service core to an application core
        MicroServiceApplication *from = (MicroServiceApplication *)src;
        cout << "Core " << dest->getCoreIndex() <<
            " recieved the response from service core " <<
            src->getCoreIndex() << "(" <<
            getServiceTypeString(from->getService()->type) << ")" << endl;
        dest->setState(MicroApplication::NORMAL);
        if(dynamic_cast<NSServiceApplication *>(src)) {
            //if the request is sent from NS
            //update src's nsCache
            NSServiceApplication *srcApp = (NSServiceApplication *)src;
            unsigned int *ns = dest->getNsCache();
            for(int i=1; i<MicroOS::nServices; i++) { //starting from 1 to exclude NS
                MicroOS::ServiceType type = (MicroOS::ServiceType) i;
                MicroOS::Service *s = MicroOS::getService(type);
                int index = srcApp->getServiceCoreIndex(type);
                ns[i] = s->runningCoreIndex[index];
            }
            dest->setNSCacheExpiration(nsCacheExpiration);
        }
        delete req;
    }
}
