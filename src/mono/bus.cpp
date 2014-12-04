#include <mono/bus.h>

void Bus::enque(CoherencyRequest *req) {
    requestQueue.push(req);
}
