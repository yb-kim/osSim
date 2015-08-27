#pragma once

#include <os.h>
#include <mono/environment.h>

class MonoEnvironment;

class MonoOS : public OS {
public:
    MonoOS(Config *cfg);
    virtual void init();
    void checkAndDoSchedule();
    void afterExecute();
    void switchApp(unsigned int coreIndex);
protected:
    void setOsSpecificSpecs(std::string osSpecificSpecs);
    int getCoherencyCost(int requestSrc, int dest);
    virtual void makeAppFactory();
    unsigned int coherencyRequestTicks;
    unsigned int coherencyRequestTicksInDie,
              coherencyRequestTicksOneHop,
              coherencyRequestTicksTwoHops;
    double inDieRatio, 
           oneHopRatio,
           twoHopsRatio;
};
