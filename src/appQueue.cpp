#include <appQueue.h>

AppQueue::AppQueue() {
    selectApp = &AppQueue::select_roundRobin;
}


AppQueue::AppQueue(SchedulePolicy policy) {
    switch(policy) {
    case ROUND_ROBIN:
        selectApp = &AppQueue::select_roundRobin;
        break;
    defalut:
        selectApp = &AppQueue::select_roundRobin;
    }
}


Application* AppQueue::select_roundRobin() {
    return new Application();
}
