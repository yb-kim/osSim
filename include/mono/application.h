#pragma once

#include <application.h>
#include <string>
#include <mono/environment.h>
#include <mono/core.h>
#include <mono/syscall.h>
#include <mono/os.h>

class CoherencyRequest;

class MonoApplication : public Application {
public:
    MonoApplication(unsigned int appSpecIndex);

    typedef enum {
        NORMAL,
        WAITING_COHERENCY_REQUEST
    } State;

    void run(unsigned int unitTick);
    void freeLock();
    bool snoopBus(CoherencyRequest *req);
    State getState() { return state; }
    void setState(State s) { state = s; }
    void setPC(int syscallIndex);
    virtual void processFinish();

    struct PC {
        unsigned int normalTicks;
        unsigned int lockTicks;
    } pc;

protected:
    unsigned int processNormalTicks(unsigned int tick); //returns ticks remaining
    unsigned int processLockTicks(unsigned int tick);
    bool getLock(int specIndex);
    bool isSyscallFinished();
    bool isSyscallWorkloadFinished();
    void moveToNextSyscallWorkload();

    State state;
    unsigned int currentSyscallWorkloadIndex;
    MonoSyscallSpec *currentSyscallSpec;
    MonoSyscallSpec::SyscallWorkload currentSyscallWorkload;
    //
};
