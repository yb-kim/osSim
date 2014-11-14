#include <micro/request.h>

using namespace std;

Request::Request(
        MicroApplication *src,
        std::list< unsigned int > *sequence,
        MicroOS *os):
    src(src), coreSequence(sequence),
    nServices(nServices), os(os)
{
    currentService = sequence->begin();
}

Request::~Request() {
    if(coreSequence) {
        delete coreSequence;
    }
}


void Request::printCoreSequence() {
    //print request sequence
    list< unsigned int >::iterator last = coreSequence->end();
    last--;
    cout << "Request path: ";
    for(list< unsigned int >::iterator iter = coreSequence->begin();
            iter != coreSequence->end();
            iter++) {
        int coreIndex = *iter;
        MicroApplication *t;
        t= (MicroApplication *)os->getEnv()->
            getCore(coreIndex)->getAppRunning();
        if(dynamic_cast <MicroServiceApplication *> (t)) {
            MicroServiceApplication *target = (MicroServiceApplication *)t;
            MicroOS::ServiceType type = target->getService()->type;
            cout << MicroOS::getServiceTypeString(type);
        }
        else {
            cout << "Application";
        }
        if(iter == currentService) {
            cout << "(*)";
        }
        else {
            cout << "(core " << coreIndex << ")";
        }
        if(iter != last) {
            cout << " -> ";
        }
    }
    cout << endl;
    return;
}
