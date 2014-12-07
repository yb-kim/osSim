#pragma once

#include <os.h>
#include <mono/environment.h>

class MonoEnvironment;

class MonoOS : public OS {
public:
    MonoOS(Config *cfg);
    void init();
    void checkAndDoSchedule();
    void afterExecute();
    void switchApp(unsigned int coreIndex);
private:
    void setOsSpecificSpecs(std::string osSpecificSpecs);
    unsigned int coherencyRequestTicks;
};
