#include <mono/coherencyRequest.h>
#include <cstdio>

using namespace std;

CoherencyRequest::CoherencyRequest(
        RequestType type, MonoCore *src,
        unsigned int syscallIndex):
    requestType(type), src(src), 
    syscallIndex(syscallIndex)
{
    cout << "constructing coherency request: " << getDescription() << endl;
}

string CoherencyRequest::getDescription() {
    char buf[100];
    string typeString = requestType == READ ? "READ" : "INVALIDATION";
    sprintf(buf, "from core %d, requestType: %s, syscallIndex: %d",
            src->getIndex(),
            typeString.c_str(),
            syscallIndex);
    string r = buf;
    return r;
}
