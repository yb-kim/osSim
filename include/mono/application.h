#include <application.h>
#include <string>
#include <mono/environment.h>
#include <mono/core.h>

class MonoApplication : public Application {
public:
    MonoApplication(unsigned int appSpecIndex);

    typedef enum {
        NORMAL,
        WAITING_COHERENCY_REQUEST
    } State;

    void run(unsigned int unitTick);
    void freeLock();

    struct PC {
        unsigned int normalTicks;
        unsigned int lockTicks;
    } pc;

private:
    unsigned int processNormalTicks(unsigned int tick); //returns ticks remaining
    unsigned int processLockTicks(unsigned int tick);
    void getLock(int specIndex);
    void setPC(unsigned int syscallIndex);
    bool isSyscallFinished();

    State state;
    //
};
