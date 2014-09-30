#pragma once

#include <micro/application.h>

class MicroApplication;

class Request {
public:
    Request() { };
    Request(MicroApplication *src, MicroApplication *dest):
        src(src), dest(dest)
    {
        //
    }

    MicroApplication *src, *dest;
};
