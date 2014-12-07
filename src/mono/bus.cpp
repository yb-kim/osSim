#include <mono/bus.h>

void Bus::enque(CoherencyRequest *req) {
    requestQueue.push(req);
}

CoherencyRequest* Bus::deque() {
    if(requestQueue.empty()) return NULL;
    CoherencyRequest *req = requestQueue.front();
    requestQueue.pop();
    return req;
}

void Bus::flushQueue() {
    std::queue<CoherencyRequest *, std::list<CoherencyRequest *> > empty;
    std::swap( requestQueue, empty );
}
