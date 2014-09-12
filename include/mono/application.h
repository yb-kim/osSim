#include <application.h>
#include <string>

class MonoApplication : public Application {
public:
    MonoApplication(unsigned int appSpecIndex);

    static void setApplications(std::string appSpecs);
    void run(unsigned int unitTick);
    void freeLock();

private:
    unsigned int processNormalTicks(unsigned int tick); //returns ticks remaining
    unsigned int processLockTicks(unsigned int tick);
    //
};
