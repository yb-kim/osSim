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
    MicroApplication(unsigned int appSpecIndex);
    virtual void run(unsigned int unitTick);
    void freeLock();

private:
    void ipc(MicroOS::Services service);
};


class MicroServiceApplication : public MicroApplication {
public:
    MicroServiceApplication();
    virtual void run(unsigned int unitTick);

private:
    MicroOS::Services service;
    queue<Request, list<Request> > requestQueue;

};


class NSServiceApplication : public MicroServiceApplication {
public:
    NSServiceApplication();
    void run(unsigned int unitTick);

private:
    //
};
