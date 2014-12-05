#pragma once

#include <mono/core.h>

class CoherencyRequest {
public:
    typedef enum {
        INVALIDATION,
        READ
    } RequestType;

    CoherencyRequest(RequestType type,
            MonoCore *src,
            unsigned int syscallIndex);

    RequestType requestType;
    MonoCore *src;
    unsigned int syscallIndex;

private:
    //
};
