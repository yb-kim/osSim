#pragma once

#include <application.h>

class AppQueue {
public:
    enum SchedulePolicy {
        ROUND_ROBIN
    };
    AppQueue();
    AppQueue(SchedulePolicy policy);
    void enque(Application *app);
    Application* deque() { return (this->*selectApp)(); }

protected:
    Application* (AppQueue::*selectApp)(void);
    Application* select_roundRobin();
};
