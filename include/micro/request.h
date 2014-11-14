#pragma once

#include <micro/application.h>
#include <micro/syscall.h>
#include <list>
#include <iostream>

class MicroApplication;
class MicroOS;

class Request {
public:
    Request() { };
    Request(MicroApplication *src, MicroApplication *dest):
        src(src), dest(dest), currentService(0),
        coreSequence(0)
    {
        //
    }

    Request(MicroApplication *src,
            std::list< unsigned int > *sequence,
            MicroOS *os);

    ~Request();
    MicroApplication *src, *dest;

    void printCoreSequence();

    std::list< unsigned int >::iterator currentService;
    int nServices;
    std::list< unsigned int > *coreSequence;
    MicroOS *os;
};
