#include <fused/type_a/request.h>

FusedOSRequest::FusedOSRequest() {
    //
}

FusedOSRequest::FusedOSRequest(int srcCoreIndex,
        int destCoreIndex, int syscallIndex, bool fromMicroOS) : 
    srcCoreIndex(srcCoreIndex), destCoreIndex(destCoreIndex), 
    syscallIndex(syscallIndex), fromMicroOS(fromMicroOS)
{
    return;
}
