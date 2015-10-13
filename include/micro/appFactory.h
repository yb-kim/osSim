#pragma once
#include <appFactory.h>
#include <micro/os.h>

class MicroAppFactory : public AppFactory {
public:
    MicroAppFactory();
    MicroAppFactory(int *appWeights, int nWeights);
    Application* createApp();
    void setOS(MicroOS *_os) { os = _os; }
    void init();

protected:
    MicroOS *os;
    int getNsCoreIndex();
    int nextNsCoreIndex;
    //
};
