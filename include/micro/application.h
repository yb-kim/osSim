#pragma once

#include <application.h>
#include <micro/os.h>
#include <micro/request.h>
#include <micro/syscall.h>
#include <queue>
#include <list>

class Request;

using namespace std;

class MicroApplication : public Application {
public:
    MicroApplication(int appSpecIndex);
    virtual void run(unsigned int unitTick);
    void freeLock();

    enum State {
        INVALID,
        NORMAL,
        WAITING_NS,
        WAITING,
        TERMINATED
    };

    void setState(State s) { state = s; }

    class PC {
    public:
        MicroSyscallSpec *spec;
        unsigned int serviceIndex;
        unsigned int normalTicks;
    } pc;

    void setOS(MicroOS *_os) { os = _os; }
    void setNSCacheExpiration(unsigned int n) { nsExpiration = n; }
    int getNSCacheExpiration() { return nsExpiration; }
    unsigned int* getNsCache() { return nsCache; }

protected:
    State state;
    MicroOS* os;
    int nsExpiration;
    //MicroOS::Service *service;
    void ipc(MicroOS::ServiceType serviceType);
    void setPC(unsigned int syscallIndex);
    unsigned int processNormalTicks(unsigned int ticks);
    
    unsigned int remainingTicks;
    bool isSyscallFinished();
    unsigned int *nsCache; //saves core index of each services

};


class MicroServiceApplication : public MicroApplication {
public:
    MicroServiceApplication();
    MicroServiceApplication(MicroOS::ServiceType serviceType);
    void enque(Request *request);
    virtual void run(unsigned int unitTick);

protected:
    MicroOS::Service* service;
    queue<Request *, list<Request *> > requestQueue;

};


class NSServiceApplication : public MicroServiceApplication {
public:
    NSServiceApplication();
    void run(unsigned int unitTick);

private:
    //
};
