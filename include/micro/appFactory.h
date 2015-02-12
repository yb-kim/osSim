#pragma once
#include <appFactory.h>
#include <micro/os.h>

class MicroAppFactory : public AppFactory {
public:
    MicroAppFactory();
    Application* createApp();
    void setOS(MicroOS *_os) { os = _os; }
    void init();

private:
    MicroOS *os;
    int getNsCoreIndex();
    int nextNsCoreIndex;
    //
};
