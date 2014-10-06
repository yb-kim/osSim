#include <application.h>
#include <string>

class MonoApplication : public Application {
public:
    MonoApplication(unsigned int appSpecIndex);

    void run(unsigned int unitTick);
    void freeLock();

    struct PC {
        unsigned int normalTicks;
        unsigned int lockTicks;
    } pc;

private:
    unsigned int processNormalTicks(unsigned int tick); //returns ticks remaining
    unsigned int processLockTicks(unsigned int tick);
    void setPC(unsigned int syscallIndex);
    bool isSyscallFinished();
    //
};
