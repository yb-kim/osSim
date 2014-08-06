#include <appQueue.h>
#include <iostream>

using namespace std;

AppQueue::AppQueue(AppQueueConfig *config):
    nWorkloads(config->getNWorkloads()),
    workloadSequence(config->getWorkloadSequence())
{
    string policyString = config->getSchedulePolicyString();

    if(policyString == "ROUND_ROBIN") schedulePolicy = AppQueue::ROUND_ROBIN;
    else schedulePolicy = AppQueue::ROUND_ROBIN; //default

    switch(schedulePolicy) {
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
