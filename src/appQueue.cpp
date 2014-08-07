#include <appQueue.h>
#include <iostream>

using namespace std;

AppQueue::AppQueue(AppQueueConfig *config):
    nWorkloads(config->getNWorkloads()),
    workloadSequence(config->getWorkloadSequence())
{
    string policyString = config->getSchedulePolicyString();
    setPolicy(policyString);
}


void AppQueue::setPolicy(string policyString) {
    if(NULL) {
        //process another scheduling
    }
    else if(policyString=="ROUND_ROBIN" || true) { //ROUND_ROBIN or default
        schedulePolicy = AppQueue::ROUND_ROBIN;
        storage = new AppQueue::StorageType<Application *>::roundRobin();
        selectApp = &AppQueue::select_roundRobin;
    }
}

Application* AppQueue::select_roundRobin() {
    AppQueue::StorageType<Application *>::roundRobin *queue;
    queue = (AppQueue::StorageType<Application *>::roundRobin *)storage;
    Application *popped = queue->front(); 
    queue->pop();
    return popped;
}

void AppQueue::enque(Application *app) {
    AppQueue::StorageType<Application *>::roundRobin *queue;
    queue = (AppQueue::StorageType<Application *>::roundRobin *)storage;
    queue->push(app);
}
