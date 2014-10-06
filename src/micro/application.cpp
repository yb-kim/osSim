#include <micro/application.h>
#include <iostream>

using namespace std;

MicroApplication::MicroApplication(int appSpecIndex) : Application(appSpecIndex) {
    remainingTicks = 0;
    state = NORMAL;
    nsExpiration = 0;
    if(appSpecIndex >= 0) setPC(syscallPointer);
}

void MicroApplication::run(unsigned int unitTick) {
    remainingTicks = unitTick;
    if(state==WAITING || state==WAITING_NS) {
        cout << "Waiting ipc result..." << endl;
        int processed = processNormalTicks(unitTick);
        cout << "normal tick processed: " << processed << endl;
        return;
    }
    //process ipc
    unsigned int nServices = pc.spec->getNServices();
    if(pc.serviceIndex <= nServices-1) {
        ipc(pc.spec->getServiceType(pc.serviceIndex));
        return;
    }
    //process normal ticks
    int processed = processNormalTicks(unitTick);
    cout << "normal tick processed: " << processed << endl;

    //move to next syscall if finished
    if(isSyscallFinished()) {
        if(!moveToNextSyscall()) { //app finished
            finished = true;
        }
    }
}

bool MicroApplication::isSyscallFinished() {
    unsigned int nServices = pc.spec->getNServices();
    return(
            pc.serviceIndex >= nServices && pc.normalTicks <= 0
          );
}

unsigned int MicroApplication::processNormalTicks(unsigned int ticks) {
    int processed = (pc.normalTicks > ticks ? ticks : pc.normalTicks);
    pc.normalTicks -= processed;
    remainingTicks -= processed;
    return processed;
}

void MicroApplication::ipc(MicroOS::ServiceType serviceType) {
    //find target
    //first look NS cache
    //if no entry in cache, make ipc to NS
    MicroServiceApplication *dest;
    if(nsExpiration <= 0) {
        cout << "ns cache expired" << endl;
        dest = (MicroServiceApplication *)os->getEnv()->getCore(0)->getAppRunning();
    } else {
        MicroOS::ServiceType type = pc.spec->getServiceType(pc.serviceIndex);
        MicroOS::Service *s = MicroOS::getService(type);
        cout << "making request to core " << s->runningCoreIndex[0] << endl;
        dest = (MicroServiceApplication *)os->getEnv()->getCore(s->runningCoreIndex[0])->getAppRunning();
    }
    Request *req = new Request(this, dest);
    dest->enque(req);
    if(nsExpiration <= 0) {
        state = WAITING_NS;
        cout << "send NS request to core 0" << endl;
    } else {
        state = WAITING;
        pc.serviceIndex++;
    }
    return;
    //make request
    //send request
}

void MicroApplication::setPC(unsigned int syscallIndex) {
    pc.spec = (MicroSyscallSpec *)Syscall::getSyscallSpec(
            spec->getSyscallIndex()[syscallIndex]);
    //pc.spec = (MicroSyscallSpec *)Syscall::getSyscallSpec(syscallIndex);
    pc.serviceIndex = 0;
    pc.normalTicks = pc.spec->getNormalTicks();
}

MicroServiceApplication::MicroServiceApplication() : MicroApplication(-1) {
    //
}

MicroServiceApplication::MicroServiceApplication(
        MicroOS::ServiceType serviceType):
    MicroApplication(-1),
    service(MicroOS::getService(serviceType))
{
    //
}

void MicroServiceApplication::enque(Request *request) {
    requestQueue.push(request);
}

void MicroServiceApplication::run(unsigned int unitTick) {
    cout << "Queue item remainaing: " << requestQueue.size() << endl;
    remainingTicks = unitTick;
    while((remainingTicks > 0) && !requestQueue.empty()) {
        Request *req = requestQueue.front();
        cout << "processing request from core " << req->src->getCoreIndex() << endl;
        requestQueue.pop();
        req->src->setState(NORMAL);
        remainingTicks -= service->ticks;
        delete req;
    }
    return;
}

NSServiceApplication::NSServiceApplication() : MicroServiceApplication() {
    service = MicroOS::getService(MicroOS::NS);
    cout << "NSServiceApp is created" << endl;
}

void NSServiceApplication::run(unsigned int unitTick) {
    cout << "Queue item remainaing: " << requestQueue.size() << endl;
    remainingTicks = unitTick;
    while((remainingTicks > 0) && !requestQueue.empty()) {
        Request *req = requestQueue.front();
        cout << "processing NS request from core " << req->src->getCoreIndex() << endl;
        requestQueue.pop();
        req->src->setState(NORMAL);
        req->src->setNSCacheExpiration(10000);
        remainingTicks -= service->ticks;
        delete req;
    }
    return;
}
