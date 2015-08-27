#pragma once

#include <application.h>
#include <micro/os.h>
#include <micro/request.h>
#include <micro/syscall.h>
#include <queue>
#include <list>

class MicroSyscallSpec;

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
    void setNsCoreIndex(int index) { nsCoreIndex = index; }
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
    void sendRequest(Request *req);
    unsigned int processNormalTicks(unsigned int ticks);
    void printSyscallStatus();
    virtual void doIpc();
    
    int remainingTicks;
    bool isSyscallFinished();
    unsigned int *nsCache; //saves core index of each services

    int nsCoreIndex;

};


class MicroServiceApplication : public MicroApplication {
public:
    MicroServiceApplication();
    MicroServiceApplication(MicroOS::ServiceType serviceType, MicroOS *os);
    void enque(Request *request);
    virtual void run(unsigned int unitTick);
    MicroOS::Service* getService() { return service; }

protected:
    MicroOS::Service* service;
    queue<Request *, list<Request *> > requestQueue;

};


class NSServiceApplication : public MicroServiceApplication {
public:
    NSServiceApplication(MicroOS *os);
    int getServiceCoreIndex(MicroOS::ServiceType type);
    //void run(unsigned int unitTick);

private:
    int nSet;
    int *indexCounter;
    //
};
