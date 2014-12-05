#include <mono/coherencyRequest.h>

CoherencyRequest::CoherencyRequest(
        RequestType type, MonoCore *src,
        unsigned int syscallIndex):
    requestType(type), src(src), 
    syscallIndex(syscallIndex)
{
    cout << "constructing coherency request: " <<
        "from core " << src->getIndex() << ", " <<
        "requestType: ";
    if(type == READ) cout << "READ, ";
    else cout << "INVALIDATION ,";
    cout << "syscallIndex: " << syscallIndex << endl;
}
