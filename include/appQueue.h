#pragma once

#include <application.h>
#include <config.h>

class AppQueueConfig;

class AppQueue {
public:
    enum SchedulePolicy {
        ROUND_ROBIN
    };
    AppQueue(AppQueueConfig *config);
    void enque(Application *app);
    Application* deque() { return (this->*selectApp)(); }

protected:
    SchedulePolicy schedulePolicy;
    int nWorkloads;
    int *workloadSequence;
    Application* (AppQueue::*selectApp)(void);
    Application* select_roundRobin();
};
