#pragma once

#include <application.h>
#include <config.h>
#include <queue>
#include <list>

class AppQueueConfig;

class AppQueue {
public:
    enum SchedulePolicy {
        ROUND_ROBIN
    };

    template<typename T>
    struct StorageType {
        typedef std::queue<T, std::list<T> > roundRobin;
    };

    AppQueue(AppQueueConfig *config);
    void enque(Application *app);
    Application* deque() { return (this->*selectApp)(); }

protected:
    SchedulePolicy schedulePolicy;
    int nWorkloads;
    void *storage;
    void setPolicy(std::string policyString);
    Application* (AppQueue::*selectApp)(void);
    Application* select_roundRobin();
};
