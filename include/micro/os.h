#pragma once

#include <os.h>
#include <vector>
#include <list>
#include <typeinfo>

class Request;

class MicroOS : public OS {
public:
    MicroOS(Config *cfg, bool initialize=true);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    enum ServiceType {
        NS, //name service
        PM,
        FM,
        DISKIO,
        NETIO,
        SERVICES_END //mark end of enumeration
    };

    class Service {
    public:
        Service(ServiceType type, int ticks):type(type), ticks(ticks) { };
        ServiceType type;
        int ticks;
        std::vector<int> runningCoreIndex;
    };

    struct RequestEntry {
        int ticks;
        Request *req;
    };

    static Service **services;
    static int nServices;
    static int nSet;

    static ServiceType getServiceType(std::string serviceString) {
        if(serviceString == "NS") return NS;
        else if(serviceString == "FM") return FM;
        else if(serviceString == "PM") return PM;
        else if(serviceString == "DISKIO") return DISKIO;
        else if(serviceString == "NETIO") return NETIO;
        else //default
            return NS;
    }

    static std::string getServiceTypeString(ServiceType serviceType) {
        switch(serviceType) {
        case NS:
            return "NS";
        case FM:
            return "FM";
        case PM:
            return "PM";
        case DISKIO:
            return "DISKIO";
        case NETIO:
            return "NETIO";
        default:
            return "UNKNOWN";
        }
    }

    static Service* getService(ServiceType serviceType);

    void getRequest(Request *req);
protected:
    std::list< RequestEntry * > waitingRequests;
    void setOsSpecificSpecs(std::string osSpecificSpecs);
    int getIpcTicks(Request *req);
    void sendRequest(Request *req);
    virtual void makeAppFactory();

    unsigned int nsCacheExpiration;
    unsigned int ipcCost_die, ipcCost_hop, ipcCost_2hops;
};

#include <micro/request.h>
