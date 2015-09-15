#pragma once
#include <appFactory.h>

class MicroOSTypeA;

class TypeAMicroAppFactory : public AppFactory {
public:
    TypeAMicroAppFactory();
    Application* createApp();
    void setOS(MicroOSTypeA *_os) { os = _os; }

private:
    MicroOSTypeA *os;
    //
};
