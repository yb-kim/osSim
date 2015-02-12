#include <micro/application.h>
#include <iostream>

using namespace std;

MicroApplication::MicroApplication(int appSpecIndex) : Application(appSpecIndex) {
    remainingTicks = 0;
    state = NORMAL;
    nsExpiration = 0;
    nsCoreIndex = 0;
    nsCache = new unsigned int[MicroOS::nServices];
    for(int i=0; i<MicroOS::nServices; i++) nsCache[i] = 0;
    if(appSpecIndex >= 0) setPC(syscallPointer);
}

void MicroApplication::run(unsigned int unitTick) {
    remainingTicks = unitTick;
    cout << "enter run()" << endl;
    cout << "Syscalls: ";
    for(int i=0; i<spec->getNSyscalls(); i++) {
        int *syscallIndex = spec->getSyscallIndex();
        MicroSyscallSpec *syscallSpec = (MicroSyscallSpec *)
            Syscall::getSyscallSpec(syscallIndex[i]);
        cout << syscallSpec->getName();
        if(syscallPointer == i) cout << "(*)";
        cout << " -> ";
    }
    cout << "TERMINATE" << endl;
    if(state==WAITING || state==WAITING_NS) {
        cout << "Waiting ipc result..." << endl;
        int processed = processNormalTicks(unitTick);
        cout << "normal tick processed: " << processed <<
            " (" << pc.normalTicks << " ticks remaining)" << endl;
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
    cout << "normal tick processed: " << processed <<
        " (" << pc.normalTicks << " ticks remaining)" << endl;

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
    cout << "Making IPC Request (";
    cout << "NS cache will expire after " << nsExpiration << " ticks)" << endl;
    MicroServiceApplication *dest;
    list< unsigned int > *sequence = new list< unsigned int>();
    sequence->push_back(coreIndex); //sender
    if(nsExpiration <= 0) {
        cout << "NS cache expired" << endl;
        sequence->push_back(nsCoreIndex);
        dest = (MicroServiceApplication *)os->getEnv()->getCore(nsCoreIndex)->getAppRunning();
    } else {
        unsigned int nServices = pc.spec->getNServices();
        for(int i=0; i<nServices; i++) {
            MicroOS::ServiceType type = pc.spec->getServiceType(i);
            sequence->push_back(nsCache[type]);
        }
        MicroOS::ServiceType type = pc.spec->getServiceType(pc.serviceIndex);
        dest = (MicroServiceApplication *)os->getEnv()->getCore(nsCache[type])->getAppRunning();
    }
    sequence->push_back(coreIndex); //final reciever

    //make request
    Request *req = new Request(this, sequence, os);
    //req = new Request(this, dest);

    //print request sequence
    req->printCoreSequence();

    //request ipc message to os
    sendRequest(req);
    if(nsExpiration <= 0) {
        state = WAITING_NS;
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

void MicroApplication::sendRequest(Request *req) {
    os->getRequest(req);
}

MicroServiceApplication::MicroServiceApplication() : MicroApplication(-1) {
    //
}

MicroServiceApplication::MicroServiceApplication(
        MicroOS::ServiceType serviceType,
        MicroOS* os):
    MicroApplication(-1),
    service(MicroOS::getService(serviceType))
{
    setOS(os);
}

void MicroServiceApplication::enque(Request *request) {
    requestQueue.push(request);
}

void MicroServiceApplication::run(unsigned int unitTick) {
    cout << "Enter run() of " << MicroOS::getServiceTypeString(service->type) << endl;
    cout << "Queue item remainaing: " << requestQueue.size() << endl;

    if(requestQueue.empty()) return; 

    //Process request
    do {
        Request *req = requestQueue.front();
        //print process information
        req->printCoreSequence();
        if(remainingTicks <= 0) {
            remainingTicks = service->ticks;
        }

        int processed = remainingTicks > unitTick ? unitTick : remainingTicks;
        remainingTicks -= processed;
        unitTick -= processed;
        cout << "Request processed: " << processed <<
            " (" << remainingTicks << " ticks remaining)" << endl;
        if(remainingTicks <= 0) {
            cout << "Request  from core " << req->src->getCoreIndex() <<
                " completed" << endl;
            os->getRequest(req);
            requestQueue.pop();
        }
    } while(!requestQueue.empty() && remainingTicks > 0);

    return;
}

NSServiceApplication::NSServiceApplication(MicroOS *os) : MicroServiceApplication() {
    setOS(os);
    service = MicroOS::getService(MicroOS::NS);
    cout << "NSServiceApp is created" << endl;
    nSet = MicroOS::nSet;
    int nServices = MicroOS::nServices;
    indexCounter = new int[nServices];
    for(int i=0; i<nServices; i++) {
        indexCounter[i] = 0;
    }
}

int NSServiceApplication::getServiceCoreIndex(MicroOS::ServiceType type) {
    if(nSet==1) return 0;

    unsigned int index = (int)type;
    int result = indexCounter[index];
    if(indexCounter[index] == nSet-1) {
        indexCounter[index]=0;
    } else {
        indexCounter[index]++;
    }
    return result;
}
