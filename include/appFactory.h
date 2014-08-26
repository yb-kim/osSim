#pragma once
#include <application.h>

class AppFactory {
public:
    AppFactory();
    virtual Application* createApp() = 0;
    //
protected:
    unsigned int selectAppType();

    unsigned int appTypePointer;
};
