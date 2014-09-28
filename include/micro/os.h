#pragma once

#include <os.h>

class MicroOS : public OS {
public:
    MicroOS(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    enum Services {
        NS, //name service
        FM,
        PM,
        SERVICES_END //mark end of enumeration
    };
    static Services getService(std::string serviceString) {
        if(serviceString == "NS") return NS;
        else if(serviceString == "FM") return FM;
        else if(serviceString == "PM") return PM;
        else //default
            return NS;
    }
private:
    //
};
