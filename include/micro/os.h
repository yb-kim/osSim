#pragma once

#include <os.h>
#include <vector>

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
private:
    void setOsSpecificSpecs(std::string osSpecificSpecs);
};
