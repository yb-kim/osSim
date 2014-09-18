#pragma once

#include <micro/application.h>

class MicroApplication;

class Request {
public:
    Request() { };

private:
    MicroApplication *src, *dest;
};
