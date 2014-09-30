#pragma once

#include <application.h>
#include <micro/os.h>
#include <micro/request.h>
#include <queue>
#include <list>

class Request;

using namespace std;

class MicroApplication : public Application {
public:
    MicroApplication(int appSpecIndex);
    virtual void run(unsigned int unitTick);
    void freeLock();

private:
    MicroOS::Service *service;
    void ipc(MicroOS::ServiceType serviceType);
};


class MicroServiceApplication : public MicroApplication {
public:
    MicroServiceApplication();
    MicroServiceApplication(MicroOS::ServiceType serviceType);
    virtual void run(unsigned int unitTick);

private:
    MicroOS::Service* service;
    queue<Request, list<Request> > requestQueue;

};


class NSServiceApplication : public MicroServiceApplication {
public:
    NSServiceApplication();
    void run(unsigned int unitTick);

private:
    //
};
