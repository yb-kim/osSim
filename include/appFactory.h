#pragma once
#include <application.h>

class AppFactory {
public:
    AppFactory();
    AppFactory(int *weights, int nApps);
    virtual Application* createApp() = 0;
    //
protected:
    unsigned int selectAppType();

    unsigned int appTypePointer;
    int *weights;
    int *accumulatedWeights;
    int weightSum;
    int nApps;
};
