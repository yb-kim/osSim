#pragma once

class FusedOSRequest {
public:
    FusedOSRequest();
    FusedOSRequest(int srcCoreIndex,
            int destCoreIndex,
            int syscallIndex,
            bool fromMicroOS);

    int srcCoreIndex;
    int destCoreIndex;
    int syscallIndex;
    bool fromMicroOS;
};
