#pragma once

#include <os.h>
#include <vector>
#include <list>
#include <typeinfo>

class Request;

class MicroOS : public OS {
public:
    MicroOS(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    enum ServiceType {
        NS, //name service
        FM,
        PM,
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
        else //default
            return NS;
    }

    static Service* getService(ServiceType serviceType);

    void getRequest(Request *req);
private:
    std::list< RequestEntry * > waitingRequests;
    void setOsSpecificSpecs(std::string osSpecificSpecs);
    int getIpcTicks(Request *req);
    void sendRequest(Request *req);
};

#include <micro/request.h>
